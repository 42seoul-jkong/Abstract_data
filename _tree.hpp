/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <algorithm.hpp>
#include <iterator.hpp>

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
              data(), color(black) {}

        _node_data(_node_data* nil, const T& data)
            : left(nil), right(nil), parent(nil),
              data(data), color(red) {}

        _node_data(const _node_data& that)
            : left(that.left), right(that.right), parent(that.parent),
              data(that.data), color(that.color) {}

        ~_node_data() {}

        _node_data& operator=(const _node_data& that)
        {
            this->left = that.left;
            this->right = that.right;
            this->parent = that.parent;
            this->data = that.data;
            this->color = that.color;
            return *this;
        }

        void change_nil(const _node_data* old_nil, _node_data* new_nil)
        {
            if (this->left == old_nil)
            {
                this->left = new_nil;
            }
            if (this->right == old_nil)
            {
                this->right = new_nil;
            }
            if (this->parent == old_nil)
            {
                this->parent = new_nil;
            }
        }
    };

    enum _tree_traversal
    {
        preorder,
        inorder,
        postorder
    };

    template <typename T, typename TTree>
    struct _tree_iterator
    {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;

        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        TTree* tree;
        typename TTree::node_type* p;

        _tree_iterator() throw()
            : p() {}

        explicit _tree_iterator(TTree* tree, typename TTree::node_type* p) throw()
            : tree(tree), p(p) {}

        _tree_iterator(const _tree_iterator& that) throw()
            : tree(that.tree), p(that.p) {}

        _tree_iterator& operator=(const _tree_iterator& that) throw()
        {
            this->tree = that.tree;
            this->p = that.p;
            return *this;
        }

        reference operator*() const throw()
        {
            return this->p->data;
        }

        pointer operator->() const throw()
        {
            return &this->p->data;
        }

        _tree_iterator& operator++() throw()
        {
            this->p = this->tree->successor(this->p);
            return *this;
        }

        _tree_iterator operator++(int) throw()
        {
            _tree_iterator tmp = *this;
            this->p = this->tree->successor(this->p);
            return tmp;
        }

        _tree_iterator& operator--() throw()
        {
            this->p = this->tree->predecessor(this->p);
            return *this;
        }

        _tree_iterator operator--(int) throw()
        {
            _tree_iterator tmp = *this;
            this->p = this->tree->predecessor(this->p);
            return tmp;
        }

        friend bool operator==(const _tree_iterator& lhs, const _tree_iterator& rhs) throw()
        {
            return lhs.p == rhs.p;
        }

        friend bool operator!=(const _tree_iterator& lhs, const _tree_iterator& rhs) throw()
        {
            return lhs.p != rhs.p;
        }
    };

    template <typename T, typename TTree>
    struct _tree_const_iterator
    {
        typedef const T value_type;
        typedef const T& reference;
        typedef const T* pointer;

        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        const TTree* tree;
        const typename TTree::node_type* p;

        _tree_const_iterator() throw()
            : p() {}

        explicit _tree_const_iterator(const TTree* tree, const typename TTree::node_type* p) throw()
            : tree(tree), p(p) {}

        _tree_const_iterator(const _tree_const_iterator& that) throw()
            : tree(that.tree), p(that.p) {}

        _tree_const_iterator(const _tree_iterator<T, TTree>& that) throw()
            : tree(that.tree), p(that.p) {}

        _tree_const_iterator& operator=(const _tree_const_iterator& that) throw()
        {
            this->tree = that.tree;
            this->p = that.p;
            return *this;
        }

        reference operator*() const throw()
        {
            return this->p->data;
        }

        pointer operator->() const throw()
        {
            return &this->p->data;
        }

        _tree_const_iterator& operator++() throw()
        {
            this->p = this->tree->successor(this->p);
            return *this;
        }

        _tree_const_iterator operator++(int) throw()
        {
            _tree_const_iterator tmp = *this;
            this->p = this->tree->successor(this->p);
            return tmp;
        }

        _tree_const_iterator& operator--() throw()
        {
            this->p = this->tree->predecessor(this->p);
            return *this;
        }

        _tree_const_iterator operator--(int) throw()
        {
            _tree_const_iterator tmp = *this;
            this->p = this->tree->predecessor(this->p);
            return tmp;
        }

        friend bool operator==(const _tree_const_iterator& lhs, const _tree_const_iterator& rhs) throw()
        {
            return lhs.p == rhs.p;
        }

        friend bool operator!=(const _tree_const_iterator& lhs, const _tree_const_iterator& rhs) throw()
        {
            return lhs.p != rhs.p;
        }
    };

    // 참조: 2-3-4 이진 탐색 트리, Red-Black 트리
    // TKeySelector: const TKey& (*keySelector)(const T&)
    // TComp: bool (*comp)(const TKey&, const TKey&)
    template <typename TKey, typename T, typename TKeySelector, typename TComp = std::less<TKey>, typename TAlloc = std::allocator<T> >
    class _tree
    {
    public:
        typedef TKey key_type;
        typedef T value_type;
        typedef _node_data<T> node_type;
        typedef TKeySelector key_selector;
        typedef TComp key_compare;
        typedef typename TAlloc::template rebind<node_type>::other allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef _tree_iterator<T, _tree> iterator;
        typedef _tree_const_iterator<T, _tree> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> reverse_const_iterator;

    private:
        // Sentinel nil
        node_type nilNode;

        // Fake root
        node_type rootNode;

        key_compare comp;
        allocator_type alloc;
        size_type count;

    public:
        _tree(const TComp& comp = TComp(), const TAlloc& alloc = TAlloc())
            : nilNode(&this->nilNode), rootNode(&this->nilNode), comp(comp), alloc(alloc), count() {}

        _tree(const _tree& that)
            : nilNode(&this->nilNode), rootNode(&this->nilNode), comp(that.comp), alloc(that.alloc), count(that.count)
        {
            this->first() = this->copy(that.first(), this->root(), that.nil());
        }

        ~_tree()
        {
            this->clear();
        }

        _tree& operator=(const _tree& that)
        {
            this->clear();
            this->first() = this->copy(that.first(), this->root(), that.nil());
            this->comp = that.comp;
            this->alloc = that.alloc;
            this->count = that.count;
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
                if (this->comp(key, key_selector()(node->data)))
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
                if (this->comp(key, key_selector()(node->data)))
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

        node_type* copy(const node_type* that, node_type* parent, const node_type* that_nil)
        {
            if (that != that_nil)
            {
                node_type* node = this->alloc.allocate(1);
                this->alloc.construct(node, *that);
                node->parent = parent;
                node->left = this->copy(that->left, node, that_nil);
                node->right = this->copy(that->right, node, that_nil);
                node->change_nil(that_nil, this->nil());
                return node;
            }
            return this->nil();
        }

        void change_nil(node_type* node, const node_type* old_nil, node_type* new_nil)
        {
            if (node != old_nil && node != new_nil)
            {
                this->change_nil(node->left, old_nil, new_nil);
                this->change_nil(node->right, old_nil, new_nil);
                node->change_nil(old_nil, new_nil);
            }
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
        iterator begin()
        {
            return iterator(this, this->minimum(first()));
        }
        const_iterator begin() const
        {
            return const_iterator(this, this->minimum(first()));
        }

        iterator end()
        {
            return iterator(this, NULL);
        }
        const_iterator end() const
        {
            return const_iterator(this, NULL);
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        reverse_const_iterator rbegin() const
        {
            return reverse_const_iterator(end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        reverse_const_iterator rend() const
        {
            return reverse_const_iterator(begin());
        }

        bool empty() const
        {
            return this->rootNode.left == &this->nilNode && this->rootNode.right == &this->nilNode;
        }

        size_type size() const { return this->count; }

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

        node_type* minimum(node_type* node)
        {
            while (node->left != this->nil())
            {
                node = node->left;
            }
            return node;
        }
        const node_type* minimum(const node_type* node) const
        {
            while (node->left != this->nil())
            {
                node = node->left;
            }
            return node;
        }

        node_type* maximum(node_type* node)
        {
            while (node->right != this->nil())
            {
                node = node->right;
            }
            return node;
        }
        const node_type* maximum(const node_type* node) const
        {
            while (node->right != this->nil())
            {
                node = node->right;
            }
            return node;
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
            if (node == NULL)
            {
                return maximum(first());
            }
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
            }
            return pred;
        }
        const node_type* predecessor(const node_type* node) const
        {
            if (node == NULL)
            {
                return maximum(first());
            }
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
            }
            return pred;
        }

        // TFunc: int (*func)(T&)
        template <typename TFunc>
        int apply_node(_tree_traversal order, node_type* node, const TFunc& func = TFunc())
        {
            TFunc f = func;
            int error = 0;

            do
            {
                if (node != this->nil())
                {
                    if (order == preorder)
                    {
                        error = f(node->data);
                        if (error != 0)
                        {
                            break;
                        }
                    }
                    error = this->apply_node<TFunc>(order, node->left, f);
                    if (error != 0)
                    {
                        break;
                    }
                    if (order == inorder)
                    {
                        error = f(node->data);
                        if (error != 0)
                        {
                            break;
                        }
                    }
                    error = this->apply_node<TFunc>(order, node->right, f);
                    if (error != 0)
                    {
                        break;
                    }
                    if (order == postorder)
                    {
                        error = f(node->data);
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
        int apply_node(_tree_traversal order, const node_type* node, const TFunc& func = TFunc()) const
        {
            TFunc f = func;
            int error = 0;

            do
            {
                if (node != this->nil())
                {
                    if (order == preorder)
                    {
                        error = f(node->data);
                        if (error != 0)
                        {
                            break;
                        }
                    }
                    error = this->apply_node<TFunc>(order, node->left, f);
                    if (error != 0)
                    {
                        break;
                    }
                    if (order == inorder)
                    {
                        error = f(node->data);
                        if (error != 0)
                        {
                            break;
                        }
                    }
                    error = this->apply_node<TFunc>(order, node->right, f);
                    if (error != 0)
                    {
                        break;
                    }
                    if (order == postorder)
                    {
                        error = f(node->data);
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
            if (parent == this->root() || this->comp(key_selector()(data), key_selector()(parent->data)))
            {
                parent->left = node;
            }
            else
            {
                parent->right = node;
            }

            repair_after_insert(node);
            this->count++;

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
            this->count--;
        }

        void clear()
        {
            this->destroy(this->first());
            this->first() = this->nil();
        }

        void move(_tree& that)
        {
            std::swap(this->first(), that.first());
            that.change_nil(that.root(), this->nil(), that.nil());
            this->change_nil(this->root(), that.nil(), this->nil());
            std::swap(this->comp, that.comp);
            std::swap(this->alloc, that.alloc);
            std::swap(this->count, that.count);
        }

        friend bool operator==(const _tree& lhs, const _tree& rhs)
        {
            return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        friend bool operator!=(const _tree& lhs, const _tree& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const _tree& lhs, const _tree& rhs)
        {
            return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

        friend bool operator<=(const _tree& lhs, const _tree& rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>(const _tree& lhs, const _tree& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator>=(const _tree& lhs, const _tree& rhs)
        {
            return !(lhs < rhs);
        }
    };
}
