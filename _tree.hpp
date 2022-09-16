/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>
#include <functional>
#include <memory>

namespace ft
{
    enum _node_color
    {
        red,
        black
    };

    template <typename T>
    struct _node_data
    {
        _node_data<T>*left, *right, *parent;
        T data;
        _node_color color;

        _node_data(_node_data* nil)
            : left(nil), right(nil), parent(nil),
              data(), color(black)
        {
        }

        _node_data(_node_data* nil, const T& data)
            : left(nil), right(nil), parent(nil),
              data(data), color(red)
        {
        }

        _node_data(const _node_data& that)
            : left(that.left), right(that.right), parent(that.parent),
              data(that.data), color(that.color)
        {
        }

        ~_node_data()
        {
        }

        _node_data& operator=(const _node_data& that)
        {
            this->left = that.left;
            this->right = that.right;
            this->parent = that.parent;
            this->data = that.data;
            this->color = that.color;
            return *this;
        }
    };

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
    class _tree
    {
    public:
        typedef TKey key_type;
        typedef T value_type;
        typedef _node_data<T> node_type;
        typedef TKeySelector key_selector;
        typedef TCompare key_compare;
        typedef typename TAlloc::template rebind<node_type>::other allocator_type;

    private:
        // Sentinel nil
        node_type nilNode;

        // Fake root
        node_type rootNode;

        allocator_type alloc;

    public:
        _tree(const TAlloc& alloc = TAlloc())
            : nilNode(&this->nilNode), rootNode(&this->nilNode), alloc(alloc)
        {
        }

        _tree(const _tree& that)
            : nilNode(&this->nilNode), rootNode(&this->nilNode), alloc(that.alloc)
        {
            this->first() = this->copy(that.first(), this->root(), that.nil());
        }

        virtual ~_tree()
        {
            this->destroy(this->first());
        }

        _tree& operator=(const _tree& that)
        {
            this->destroy(this->first());
            this->first() = this->copy(that.first(), this->root(), that.nil());
            return *this;
        }

    protected:
        bool try_find(const key_type& key, node_type*& node, node_type*& parent)
        {
            node = this->first();
            parent = this->root();

            while (node != this->nil())
            {
                parent = node;
                if (key == key_selector()(node->data))
                {
                    return true;
                }
                if (key_compare()(key, key_selector()(node->data)))
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
        bool try_find(const key_type& key, const node_type*& node, const node_type*& parent) const
        {
            node = this->first();
            parent = this->root();

            while (node != this->nil())
            {
                parent = node;
                if (key == key_selector()(node->data))
                {
                    return true;
                }
                if (key_compare()(key, key_selector()(node->data)))
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

        void rotate_left(node_type* node)
        {
            node_type* child = node->right;
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

        void rotate_right(node_type* node)
        {
            node_type* child = node->left;
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

        void repair_after_insert(node_type* node)
        {
            node_type* uncle;

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

        void repair_before_delete(node_type* node)
        {
            node_type* sibling;

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

        node_type* copy(node_type* that, node_type* parent, node_type* that_nil)
        {
            if (that != that_nil)
            {
                node_type* node = this->alloc.allocate(1);
                this->alloc.construct(node, *that);
                node->parent = parent;
                node->left = this->copy(that->left, node, that_nil);
                node->right = this->copy(that->right, node, that_nil);
                return node;
            }
            return this->nil();
        }

        void destroy(node_type* node)
        {
            if (node != this->nil())
            {
                this->destroy(node->left);
                this->destroy(node->right);
                this->alloc.destroy(node);
                this->alloc.deallocate(node, 1);
            }
        }

    public:
        bool empty() const
        {
            return this->rootNode.left == &this->nilNode && this->rootNode.right == &this->nilNode;
        }

        node_type*& first() { return this->rootNode.left; }
        node_type* const& first() const { return this->rootNode.left; }

        node_type* root() { return &this->rootNode; }
        const node_type* root() const { return &this->rootNode; }

        node_type* nil() { return &this->nilNode; }
        const node_type* nil() const { return &this->nilNode; }

        node_type* find(const key_type& key)
        {
            node_type* node;
            node_type* parent;

            if (try_find(key, node, parent))
            {
                return node;
            }
            else
            {
                return NULL;
            }
        }
        const node_type* find(const key_type& key) const
        {
            const node_type* node;
            const node_type* parent;

            if (try_find(key, node, parent))
            {
                return node;
            }
            else
            {
                return NULL;
            }
        }

        node_type* successor(node_type* node)
        {
            node_type* succ = node->right;
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
        const node_type* successor(const node_type* node) const
        {
            const node_type* succ = node->right;
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

        node_type* predecessor(node_type* node)
        {
            node_type* pred = node->left;
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
        const node_type* predecessor(const node_type* node) const
        {
            const node_type* pred = node->left;
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
        int apply_node(_tree_traversal order, node_type* node)
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
        int apply_node(_tree_traversal order, const node_type* node) const
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

        bool insert_node(const value_type& data)
        {
            node_type* node;
            node_type* parent;

            if (try_find(key_selector()(data), node, parent))
            {
                return false;
            }

            node = this->alloc.allocate(1);
            this->alloc.construct(node, node_type(this->nil(), data));
            node->parent = parent;
            if (parent == this->root() || key_compare()(key_selector()(data), key_selector()(parent->data)))
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

        void delete_node(node_type* z)
        {
            node_type *x, *y;

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
            this->alloc.destroy(z);
            this->alloc.deallocate(z, 1);
        }
    };
}
