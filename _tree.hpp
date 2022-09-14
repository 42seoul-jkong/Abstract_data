/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>
#include <functional>
#include <memory>

enum _tree_traversal
{
    preorder,
    inorder,
    postorder
};

// 참조: 2-3-4 이진 탐색 트리, Red-Black 트리
// TKeySelector: const TKey& (*keySelector)(const T&)
// TCompare: bool (*compare)(const TKey&, const TKey&)
template <typename TKey, typename T, typename TKeySelector, typename TCompare = std::less<TKey>, typename TAlloc = std::allocator<T> >
class _red_black_tree
{
public:
    typedef TKey key_type;
    typedef T value_type;
    typedef TKeySelector key_selector;
    typedef TCompare key_compare;
    typedef TAlloc allocator_type;

    enum node_color
    {
        red,
        black
    };

    struct node_data
    {
        node_data *left, *right, *parent;
        T data;
        node_color color;

        node_data(node_data* nil)
            : left(nil), right(nil), parent(nil),
              data(), color(black)
        {
        }

        node_data(node_data* nil, const T& data)
            : left(nil), right(nil), parent(nil),
              data(data), color(red)
        {
        }

        node_data(const node_data& that)
            : left(that.left), right(that.right), parent(that.parent),
              data(that.data), color(that.color)
        {
        }

        ~node_data()
        {
        }

        node_data& operator=(const node_data& that)
        {
            this->left = that.left;
            this->right = that.right;
            this->parent = that.parent;
            this->data = that.data;
            this->color = that.color;
            return *this;
        }
    };

private:
    // Fake root
    node_data rootNode;

    // Sentinel nil
    node_data nilNode;

public:
    _red_black_tree()
        : nilNode(&this->nilNode), rootNode(&this->nilNode)
    {
    }

    _red_black_tree(const _red_black_tree& that)
        : nilNode(&this->nilNode), rootNode(&this->nilNode)
    {
        this->first() = this->copy(that.first(), this->root());
    }

    virtual ~_red_black_tree()
    {
        this->destroy(this->first());
    }

    _red_black_tree& operator=(const _red_black_tree& that)
    {
        this->destroy(this->first());
        this->first() = this->copy(that.first(), this->root());
        return *this;
    }

protected:
    bool try_find(const TKey& key, node_data*& node, node_data*& parent)
    {
        node = this->first();
        parent = this->root();

        while (node != this->nil())
        {
            parent = node;
            if (key == TKeySelector()(node->data))
            {
                return true;
            }
            if (TCompare()(key, TKeySelector()(node->data)))
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        return false;
    }
    bool try_find(const TKey& key, const node_data*& node, const node_data*& parent) const
    {
        node = this->first();
        parent = this->root();

        while (node != this->nil())
        {
            parent = node;
            if (key == TKeySelector()(node->data))
            {
                return true;
            }
            if (TCompare()(key, TKeySelector()(node->data)))
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        return false;
    }

    void rotate_left(node_data* node)
    {
        node_data* child = node->right;
        node->right = child->left;

        if (child->left != this->nil())
        {
            child->left->parent = node;
        }
        child->parent = node->parent;

        if (node == node->parent->left)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }
        child->left = node;
        node->parent = child;
    }

    void rotate_right(node_data* node)
    {
        node_data* child = node->left;
        node->left = child->right;

        if (child->right != this->nil())
        {
            child->right->parent = node;
        }
        child->parent = node->parent;

        if (node == node->parent->left)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }
        child->right = node;
        node->parent = child;
    }

    void repair_after_insert(node_data* node)
    {
        node_data* uncle;

        while (node->parent->color == red)
        {
            if (node->parent == node->parent->parent->left)
            {
                uncle = node->parent->parent->right;
                if (uncle->color == red)
                {
                    node->parent->color = black;
                    uncle->color = black;
                    node->parent->parent->color = red;
                    node = node->parent->parent;
                }
                else // if (uncle->color == black)
                {
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        this->rotate_left(node);
                    }
                    node->parent->color = black;
                    node->parent->parent->color = red;
                    this->rotate_right(node->parent->parent);
                }
            }
            else // if (node->parent == node->parent->parent->right)
            {
                uncle = node->parent->parent->left;
                if (uncle->color == red)
                {
                    node->parent->color = black;
                    uncle->color = black;
                    node->parent->parent->color = red;
                    node = node->parent->parent;
                }
                else // if (uncle->color == black)
                {
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        this->rotate_right(node);
                    }
                    node->parent->color = black;
                    node->parent->parent->color = red;
                    this->rotate_left(node->parent->parent);
                }
            }
        }

        this->first()->color = black;
    }

    void repair_before_delete(node_data* node)
    {
        node_data* sibling;

        while (node->color == black && node != this->first())
        {
            if (node == node->parent->left)
            {
                sibling = node->parent->right;
                if (sibling->color == red)
                {
                    sibling->color = black;
                    node->parent->color = red;
                    this->rotate_left(node->parent);
                    sibling = node->parent->right;
                }
                if (sibling->right->color == black && sibling->left->color == black)
                {
                    sibling->color = red;
                    node = node->parent;
                }
                else
                {
                    if (sibling->right->color == black)
                    {
                        sibling->left->color = black;
                        sibling->color = red;
                        this->rotate_right(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = black;
                    sibling->right->color = black;
                    this->rotate_left(node->parent);
                    node = this->first(); // break;
                }
            }
            else // if (node == node->parent->right)
            {
                sibling = node->parent->left;
                if (sibling->color == red)
                {
                    sibling->color = black;
                    node->parent->color = red;
                    rotate_right(node->parent);
                    sibling = node->parent->left;
                }
                if (sibling->right->color == black && sibling->left->color == black)
                {
                    sibling->color = red;
                    node = node->parent;
                }
                else
                {
                    if (sibling->left->color == black)
                    {
                        sibling->right->color = black;
                        sibling->color = red;
                        this->rotate_left(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = black;
                    sibling->left->color = black;
                    this->rotate_right(node->parent);
                    node = this->first(); // break;
                }
            }
        }
        node->color = black;
    }

    node_data* copy(node_data* that, node_data* parent)
    {
        if (that != that->parent) // is not nil
        {
            node_data* node = new node_data(*that);
            node->parent = parent;
            node->left = this->copy(that->left, node);
            node->right = this->copy(that->right, node);
            return node;
        }
        return this->nil();
    }

    void destroy(node_data* node)
    {
        if (node != this->nil())
        {
            this->destroy(node->left);
            this->destroy(node->right);
            delete node;
        }
    }

public:
    bool empty() const
    {
        return this->rootNode.left == &this->nilNode && this->rootNode.right == &this->nilNode;
    }

    node_data*& first() { return this->rootNode.left; }
    node_data* const& first() const { return this->rootNode.left; }

    node_data* root() { return &this->rootNode; }
    const node_data* root() const { return &this->rootNode; }

    node_data* nil() { return &this->nilNode; }
    const node_data* nil() const { return &this->nilNode; }

    node_data* find(const TKey& key)
    {
        node_data* node;
        node_data* parent;

        if (try_find(key, node, parent))
        {
            return node;
        }
        else
        {
            return NULL;
        }
    }
    const node_data* find(const TKey& key) const
    {
        const node_data* node;
        const node_data* parent;

        if (try_find(key, node, parent))
        {
            return node;
        }
        else
        {
            return NULL;
        }
    }

    node_data* successor(node_data* node)
    {
        node_data* succ = node->right;
        if (succ != this->nil())
        {
            while (succ->left != this->nil())
            {
                succ = succ->left;
            }
        }
        else
        {
            for (succ = node->parent; node == succ->right; succ = succ->parent)
            {
                node = succ;
            }
            if (succ == this->root())
            {
                succ = NULL;
            }
        }
        return succ;
    }
    const node_data* successor(const node_data* node) const
    {
        const node_data* succ = node->right;
        if (succ != this->nil())
        {
            while (succ->left != this->nil())
            {
                succ = succ->left;
            }
        }
        else
        {
            for (succ = node->parent; node == succ->right; succ = succ->parent)
            {
                node = succ;
            }
            if (succ == this->root())
            {
                succ = NULL;
            }
        }
        return succ;
    }

    node_data* predecessor(node_data* node)
    {
        node_data* pred = node->left;
        if (pred != this->nil())
        {
            while (pred->right != this->nil())
            {
                pred = pred->right;
            }
        }
        else
        {
            for (pred = node->parent; node == pred->left; pred = pred->parent)
            {
                node = pred;
            }
            if (pred == this->root())
            {
                pred = NULL;
            }
        }
        return pred;
    }
    const node_data* predecessor(const node_data* node) const
    {
        const node_data* pred = node->left;
        if (pred != this->nil())
        {
            while (pred->right != this->nil())
            {
                pred = pred->right;
            }
        }
        else
        {
            for (pred = node->parent; node == pred->left; pred = pred->parent)
            {
                node = pred;
            }
            if (pred == this->root())
            {
                pred = NULL;
            }
        }
        return pred;
    }

    // TFunc: int (*func)(T&)
    template <typename TFunc>
    int apply_node(_tree_traversal order, node_data* node)
    {
        int error = 0;

        do
        {
            if (node != this->nil())
            {
                if (order == preorder)
                {
                    error = TFunc()(node->data);
                    if (error != 0)
                    {
                        break;
                    }
                }
                error = this->apply_node<TFunc>(order, node->left);
                if (error != 0)
                {
                    break;
                }
                if (order == inorder)
                {
                    error = TFunc()(node->data);
                    if (error != 0)
                    {
                        break;
                    }
                }
                error = this->apply_node<TFunc>(order, node->right);
                if (error != 0)
                {
                    break;
                }
                if (order == postorder)
                {
                    error = TFunc()(node->data);
                    if (error != 0)
                    {
                        break;
                    }
                }
            }
        } while (0);
        return error;
    }
    // TFunc: int (*func)(const T&)
    template <typename TFunc>
    int apply_node(_tree_traversal order, const node_data* node) const
    {
        int error = 0;

        do
        {
            if (node != this->nil())
            {
                if (order == preorder)
                {
                    error = TFunc()(node->data);
                    if (error != 0)
                    {
                        break;
                    }
                }
                error = this->apply_node<TFunc>(order, node->left);
                if (error != 0)
                {
                    break;
                }
                if (order == inorder)
                {
                    error = TFunc()(node->data);
                    if (error != 0)
                    {
                        break;
                    }
                }
                error = this->apply_node<TFunc>(order, node->right);
                if (error != 0)
                {
                    break;
                }
                if (order == postorder)
                {
                    error = TFunc()(node->data);
                    if (error != 0)
                    {
                        break;
                    }
                }
            }
        } while (0);
        return error;
    }

    bool insert_node(const T& data)
    {
        node_data* node;
        node_data* parent;

        if (try_find(TKeySelector()(data), node, parent))
        {
            return false;
        }

        node = new node_data(this->nil(), data);
        node->parent = parent;
        if (parent == this->root() || TCompare()(TKeySelector()(data), TKeySelector()(parent->data)))
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }

        repair_after_insert(node);

        return true;
    }

    void delete_node(node_data* z)
    {
        node_data *x, *y;

        if (z->left == this->nil() || z->right == this->nil())
        {
            y = z;
        }
        else
        {
            y = this->successor(z);
        }
        if (y->left != this->nil())
        {
            x = y->left;
        }
        else
        {
            x = y->right;
        }

        x->parent = y->parent;
        if (x->parent == this->root())
        {
            this->first() = x;
        }
        else
        {
            if (y == y->parent->left)
            {
                y->parent->left = x;
            }
            else
            {
                y->parent->right = x;
            }
        }
        if (y->color == black)
        {
            this->repair_before_delete(x);
        }
        if (y != z)
        {
            y->left = z->left;
            y->right = z->right;
            y->parent = z->parent;
            y->color = z->color;
            z->left->parent = z->right->parent = y;
            if (z == z->parent->left)
            {
                z->parent->left = y;
            }
            else
            {
                z->parent->right = y;
            }
        }
        delete z;
    }
};
