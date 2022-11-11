/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <algorithm.hpp>
#include <iterator.hpp>
#include <utility.hpp>

#include <cstddef>
#include <functional>
#include <limits>
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

        _node_data()
            : left(), right(), parent(),
              data(), color(black) {}

        _node_data(const T& data)
            : left(), right(), parent(),
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
    };

    enum _tree_traversal
    {
        preorder,
        inorder,
        postorder
    };

    enum _find_spec
    {
        matchingnode,
        parentnode,
        lowerboundnode,
        upperboundnode
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

        typename TTree::node_type* base() const throw()
        {
            return this->p;
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

        const typename TTree::node_type* base() const throw()
        {
            return this->p;
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
        node_type nilNode;
        node_type rootNode;

        key_compare comp;
        allocator_type alloc;
        size_type count;

    public:
        _tree(const TComp& comp = TComp(), const TAlloc& alloc = TAlloc())
            : nilNode(), rootNode(), comp(comp), alloc(alloc), count()
        {
            this->make_sentinel(&this->nilNode);
        }

        _tree(const _tree& that)
            : nilNode(), rootNode(), comp(that.comp), alloc(that.alloc), count(that.count)
        {
            this->make_sentinel(&this->nilNode);
            this->first() = this->copy(that.first(), this->root());
        }

        ~_tree()
        {
            this->clear();
        }

        _tree& operator=(const _tree& that)
        {
            this->clear();
            this->first() = this->copy(that.first(), this->root());
            this->comp = that.comp;
            this->alloc = that.alloc;
            this->count = that.count;
            return *this;
        }

    public:
        allocator_type get_allocator() const { return this->alloc; }
        key_compare key_comp() const { return this->comp; }

        node_type*& first() { return this->rootNode.left; }
        node_type* const& first() const { return this->rootNode.left; }

    protected:
        node_type* root() { return &this->rootNode; }
        const node_type* root() const { return &this->rootNode; }

        node_type* safe(node_type* node) { return node ? node : &this->nilNode; }
        const node_type* safe(const node_type* node) const { return node ? node : &this->nilNode; }

        node_type* find_specified(_find_spec spec, node_type* node, const key_type& key) { return const_cast<node_type*>(const_cast<const _tree*>(this)->find_specified(spec, node, key)); }
        const node_type* find_specified(_find_spec spec, const node_type* node, const key_type& key) const
        {
            const node_type* result = NULL;
            if (spec == parentnode)
            {
                if (node != NULL)
                {
                    result = node->parent;
                }
                else
                {
                    result = this->root();
                }
            }

            while (node != NULL)
            {
                if (spec == matchingnode)
                {
                    if (key == key_selector()(node->data))
                    {
                        result = node;
                        break;
                    }
                }
                if (spec == parentnode)
                {
                    result = node;
                    if (key == key_selector()(node->data))
                    {
                        break;
                    }
                }
                if (this->comp(key, key_selector()(node->data)))
                {
                    if (spec == upperboundnode)
                    {
                        result = node;
                    }
                    node = node->left;
                }
                else
                {
                    if (spec == lowerboundnode)
                    {
                        result = node;
                    }
                    node = node->right;
                }
            }
            return result;
        }

        void rotate_left(node_type* node)
        {
            node_type* child = node->right;
            node->right = child->left;

            if (child->left != NULL)
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

            if (child->right != NULL)
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
                    uncle = this->safe(node->parent->parent->right);
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
                    uncle = this->safe(node->parent->parent->left);
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
                        this->rotate_right(node->parent);
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

        void make_sentinel(node_type* node)
        {
            node->parent = node;
            node->left = node;
            node->right = node;
        }

        node_type* copy(const node_type* that, node_type* parent)
        {
            if (that != NULL)
            {
                node_type* node = this->alloc.allocate(1);
                this->alloc.construct(node, *that);
                node->parent = parent;
                node->left = this->copy(that->left, node);
                node->right = this->copy(that->right, node);
                return node;
            }
            return NULL;
        }

        void destroy(node_type* node)
        {
            if (node != NULL)
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
            if (this->first() != NULL)
            {
                return iterator(this, this->minimum(this->first()));
            }
            else
            {
                return end();
            }
        }
        const_iterator begin() const
        {
            if (this->first() != NULL)
            {
                return const_iterator(this, this->minimum(this->first()));
            }
            else
            {
                return end();
            }
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
            return reverse_iterator(this->end());
        }
        reverse_const_iterator rbegin() const
        {
            return reverse_const_iterator(this->end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(this->begin());
        }
        reverse_const_iterator rend() const
        {
            return reverse_const_iterator(this->begin());
        }

        bool empty() const
        {
            return this->rootNode.left == NULL && this->rootNode.right == NULL;
        }

        size_type size() const { return this->count; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / (2 * sizeof(node_type)); }

        node_type* find(const key_type& key) { return const_cast<node_type*>(const_cast<const _tree*>(this)->find(key)); }
        const node_type* find(const key_type& key) const
        {
            return this->find_specified(matchingnode, this->first(), key);
        }

        node_type* lower_bound(const key_type& key) { return const_cast<node_type*>(const_cast<const _tree*>(this)->lower_bound(key)); }
        const node_type* lower_bound(const key_type& key) const
        {
            return this->find_specified(lowerboundnode, this->first(), key);
        }

        node_type* upper_bound(const key_type& key) { return const_cast<node_type*>(const_cast<const _tree*>(this)->upper_bound(key)); }
        const node_type* upper_bound(const key_type& key) const
        {
            return this->find_specified(upperboundnode, this->first(), key);
        }

        node_type* minimum(node_type* node) { return const_cast<node_type*>(const_cast<const _tree*>(this)->minimum(node)); }
        const node_type* minimum(const node_type* node) const
        {
            while (node->left != NULL)
            {
                node = node->left;
            }
            return node;
        }

        node_type* maximum(node_type* node) { return const_cast<node_type*>(const_cast<const _tree*>(this)->maximum(node)); }
        const node_type* maximum(const node_type* node) const
        {
            while (node->right != NULL)
            {
                node = node->right;
            }
            return node;
        }

        node_type* successor(node_type* node) { return const_cast<node_type*>(const_cast<const _tree*>(this)->successor(node)); }
        const node_type* successor(const node_type* node) const
        {
            const node_type* succ = node->right;
            if (succ != NULL)
            {
                while (succ->left != NULL)
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

        node_type* predecessor(node_type* node) { return const_cast<node_type*>(const_cast<const _tree*>(this)->predecessor(node)); }
        const node_type* predecessor(const node_type* node) const
        {
            if (node == NULL)
            {
                return this->maximum(this->first());
            }

            const node_type* pred = node->left;
            if (pred != NULL)
            {
                while (pred->right != NULL)
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
                if (node != NULL)
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
                if (node != NULL)
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

        ft::pair<iterator, bool> insert_node(const value_type& data)
        {
            node_type* parent = this->find_specified(parentnode, this->first(), key_selector()(data));
            node_type* node = this->find_specified(matchingnode, parent, key_selector()(data));
            if (parent != this->root() && node != NULL)
            {
                return ft::make_pair(iterator(this, node), false);
            }

            node = this->alloc.allocate(1);
            this->alloc.construct(node, node_type(data));
            node->parent = parent;
            if (parent == this->root() || this->comp(key_selector()(data), key_selector()(parent->data)))
            {
                parent->left = node;
            }
            else
            {
                parent->right = node;
            }

            this->repair_after_insert(node);
            this->count++;

            return ft::make_pair(iterator(this, node), true);
        }

        void delete_node(node_type* z)
        {
            node_type *x, *y;

            if (z->left == NULL || z->right == NULL)
            {
                y = z;
            }
            else
            {
                y = this->successor(z);
            }
            if (y->left != NULL)
            {
                x = y->left;
            }
            else // if (y->right != NULL)
            {
                x = y->right;
            }

            if (x != NULL)
            {
                x->parent = y->parent;
            }
            if (y->parent == this->root())
            {
                this->first() = x;
            }
            else
            {
                if (y == y->parent->left)
                {
                    y->parent->left = x;
                }
                else // if (y == y->parent->right)
                {
                    y->parent->right = x;
                }
            }
            if (y->color == black)
            {
                node_type safe_x;
                if (x == NULL)
                {
                    safe_x.parent = y->parent;
                    x = &safe_x;
                }
                this->repair_before_delete(x);
            }
            if (y != z)
            {
                y->left = z->left;
                y->right = z->right;
                y->parent = z->parent;
                y->color = z->color;
                if (z->left != NULL)
                {
                    z->left->parent = y;
                }
                if (z->right != NULL)
                {
                    z->right->parent = y;
                }
                if (z == z->parent->left)
                {
                    z->parent->left = y;
                }
                else // if (z == z->parent->right)
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
            this->first() = NULL;
            this->count = size_type();
        }

        void swap(_tree& that)
        {
            std::swap(this->first(), that.first());
            if (this->first() != NULL)
            {
                this->first()->parent = this->root();
            }
            if (that.first() != NULL)
            {
                that.first()->parent = that.root();
            }
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
