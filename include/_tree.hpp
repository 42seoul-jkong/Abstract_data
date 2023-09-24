/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "algorithm.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "utility.hpp"

#include <cstddef>
#include <limits>
#include <memory>

#ifdef FT_TREE_ASSERT
#include <cassert>
#endif

namespace ft
{
    enum _node_color
    {
        black,
        red
    };

    struct _node
    {
        typedef _node* pointer_type;

        pointer_type left, right, parent;
        _node_color color;

        explicit _node(_node_color color = black)
            : left(), right(), parent(),
              color(color) {}

        _node(const _node& that)
            : left(that.left), right(that.right), parent(that.parent),
              color(that.color) {}

        ~_node() {}

        _node& operator=(const _node& that)
        {
            this->left = that.left;
            this->right = that.right;
            this->parent = that.parent;
            this->color = that.color;
            return *this;
        }
    };

    template <typename T>
    struct _node_with_data : _node
    {
        typedef _node_with_data<T>* pointer_type;

        T data;

        explicit _node_with_data(const T& data)
            : _node(red), data(data) {}

        _node_with_data(const _node_with_data& that)
            : _node(that), data(that.data) {}

        ~_node_with_data() {}

        _node_with_data& operator=(const _node_with_data& that)
        {
            this->_node::operator=(that);
            this->data = that.data;
            this->color = that.color;
            return *this;
        }
    };

    // 참조: 2-3-4 이진 탐색 트리, Red-Black 트리
    struct _tree_algorithm
    {
        typedef _node::pointer_type node_pointer;

        // BEGIN Binary Search Tree
        static node_pointer successor(node_pointer node)
        {
            node_pointer succ = node->right;
            if (succ != NULL)
            {
                succ = minimum(succ);
            }
            else
            {
                for (succ = node->parent; node == succ->right; succ = succ->parent)
                {
                    node = succ;
                }
                if (node->right == succ)
                {
                    // node is header and tree of size 1
                    succ = node;
                }
            }
            return succ;
        }

        static node_pointer predecessor(node_pointer node)
        {
            if (is_header(node))
            {
                // maximum of tree
                return node->right;
            }

            node_pointer pred = node->left;
            if (pred != NULL)
            {
                pred = maximum(pred);
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

        static node_pointer minimum(node_pointer node)
        {
            while (node->left != NULL)
            {
                node = node->left;
            }
            return node;
        }

        static node_pointer maximum(node_pointer node)
        {
            while (node->right != NULL)
            {
                node = node->right;
            }
            return node;
        }

        static bool is_header(node_pointer node)
        {
            if (node->parent == NULL)
            {
                // header of empty tree
                return true;
            }

            node_pointer left = node->left;
            node_pointer right = node->right;
            if (left && right)
            {
                // header always has following children.
                // left: minimum, right: maximum
                if (left == right)
                {
                    // header for tree of size 1
                    return true;
                }
                if (!(left->parent == node && right->parent == node))
                {
                    // children of common nodes have the same parents.
                    // but minimum and maximum cannot have same parent.
                    return true;
                }
            }

            return false;
        }

        static node_pointer get_header(node_pointer node)
        {
            if (node->parent == NULL)
            {
                // header of empty node
                return node;
            }

            if (node->parent->parent == node)
            {
                // header or root node
                if (is_header(node))
                {
                    return node;
                }
                else
                {
                    return node->parent;
                }
            }

            node_pointer head;
            for (head = node->parent; node == head->parent; head = head->parent)
            {
                node = head;
            }
            return head;
        }

        static void set_child(node_pointer parent_node, bool left, node_pointer child_node, node_pointer header)
        {
            if (parent_node == header)
            {
                header->parent = child_node;
            }
            else if (left)
            {
                parent_node->left = child_node;
            }
            else // if (!left)
            {
                parent_node->right = child_node;
            }
        }

        static void rotate_left_raw(node_pointer node, node_pointer node_right)
        {
            node_pointer node_right_left = node_right->left;

            node->right = node_right_left;
            if (node_right_left != NULL)
            {
                node_right_left->parent = node;
            }

            node_right->left = node;
            node->parent = node_right;
        }

        static void rotate_left(node_pointer node, node_pointer node_right, node_pointer node_parent, node_pointer header)
        {
            bool left = node == node_parent->left;
            rotate_left_raw(node, node_right);

            node_right->parent = node_parent;
            set_child(node_parent, left, node_right, header);
        }

        static void rotate_right_raw(node_pointer node, node_pointer node_left)
        {
            node_pointer node_left_right = node_left->right;

            node->left = node_left_right;
            if (node_left_right != NULL)
            {
                node_left_right->parent = node;
            }

            node_left->right = node;
            node->parent = node_left;
        }

        static void rotate_right(node_pointer node, node_pointer node_left, node_pointer node_parent, node_pointer header)
        {
            bool left = node == node_parent->left;
            rotate_right_raw(node, node_left);

            node_left->parent = node_parent;
            set_child(node_parent, left, node_left, header);
        }
        // END Binary Search Tree

        static void repair_after_insert(node_pointer header, node_pointer node)
        {
            node->color = red;
            for (;;)
            {
                node_pointer parent = node->parent;
                if (parent == header || parent->color == black)
                {
                    break;
                }

                node_pointer grandparent = parent->parent;
                if (grandparent == header)
                {
                    break;
                }

                node_pointer uncle;
                if (parent == grandparent->left)
                {
                    uncle = grandparent->right;
                }
                else
                {
                    uncle = grandparent->left;
                }

                grandparent->color = red;
                if (uncle != NULL && uncle->color == red)
                {
                    uncle->color = black;
                    parent->color = black;
                    node = grandparent;
                }
                else // if (uncle->color == black) aka final step
                {
                    if (parent == grandparent->left)
                    {
                        if (node == parent->right)
                        {
                            rotate_left_raw(parent, node);
                            parent = node;
                        }
                        rotate_right(grandparent, parent, grandparent->parent, header);
                    }
                    else
                    {
                        if (node == parent->left)
                        {
                            rotate_right_raw(parent, node);
                            parent = node;
                        }
                        rotate_left(grandparent, parent, grandparent->parent, header);
                    }
                    parent->color = black;
                    break;
                }
            }
            header->parent->color = black;
        }

        static void repair_after_erase(node_pointer header, node_pointer z, node_pointer y, node_pointer x, node_pointer x_parent)
        {
            _node_color z_color;
            if (y == z)
            {
                z_color = z->color;
            }
            else
            {
                z_color = y->color;
                y->color = z->color;
            }

            if (z_color == red)
            {
                return;
            }

            for (;;)
            {
                if (x_parent == header)
                {
                    break;
                }

                if (x != NULL && x->color != black)
                {
                    break;
                }

                node_pointer sibling;
                if (x == x_parent->left)
                {
                    sibling = x_parent->right;
                    if (sibling->color == red)
                    {
                        sibling->color = black;
                        x_parent->color = red;
                        rotate_left(x_parent, sibling, x_parent->parent, header);
                        sibling = x_parent->right;
                        // assert(sibling != NULL);
                    }

                    node_pointer s_left = sibling->left;
                    node_pointer s_right = sibling->right;
                    if ((s_left == NULL || s_left->color == black) && (s_right == NULL || s_right->color == black))
                    {
                        sibling->color = red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if (s_right == NULL || s_right->color == black)
                        {
                            s_left->color = black;
                            sibling->color = red;
                            rotate_right(sibling, s_left, sibling->parent, header);
                            sibling = x_parent->right;
                            // assert(sibling != NULL);
                        }

                        sibling->color = x_parent->color;
                        x_parent->color = black;
                        s_right = sibling->right;
                        if (s_right != NULL)
                        {
                            s_right->color = black;
                        }
                        rotate_left(x_parent, x_parent->right, x_parent->parent, header);
                        break;
                    }
                }
                else // if (x == x_parent->right)
                {
                    sibling = x_parent->left;
                    if (sibling->color == red)
                    {
                        sibling->color = black;
                        x_parent->color = red;
                        rotate_right(x_parent, sibling, x_parent->parent, header);
                        sibling = x_parent->left;
                        // assert(sibling != NULL);
                    }

                    node_pointer s_left = sibling->left;
                    node_pointer s_right = sibling->right;
                    if ((s_left == NULL || s_left->color == black) && (s_right == NULL || s_right->color == black))
                    {
                        sibling->color = red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if (s_left == NULL || s_left->color == black)
                        {
                            s_right->color = black;
                            sibling->color = red;
                            rotate_left(sibling, s_right, sibling->parent, header);
                            sibling = x_parent->left;
                            // assert(sibling != NULL);
                        }

                        sibling->color = x_parent->color;
                        x_parent->color = black;
                        s_left = sibling->left;
                        if (s_left != NULL)
                        {
                            s_left->color = black;
                        }
                        rotate_right(x_parent, x_parent->left, x_parent->parent, header);
                        break;
                    }
                }
            }

            if (x != NULL)
            {
                x->color = black;
            }
        }
    };

    template <typename T, typename TTree>
    struct _tree_iterator
    {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;

        typedef ft::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        typename TTree::node_type::pointer_type p;

        _tree_iterator() throw()
            : p() {}

        explicit _tree_iterator(typename TTree::node_type* p) throw()
            : p(p) {}

        _tree_iterator(const _tree_iterator& that) throw()
            : p(that.p) {}

        _tree_iterator& operator=(const _tree_iterator& that) throw()
        {
            this->p = that.p;
            return *this;
        }

        typename TTree::node_type* base() const throw()
        {
            return this->p;
        }

        reference operator*() const throw()
        {
            return static_cast<typename TTree::node_with_value_type::pointer_type>(this->p)->data;
        }

        pointer operator->() const throw()
        {
            return &static_cast<typename TTree::node_with_value_type::pointer_type>(this->p)->data;
        }

        _tree_iterator& operator++() throw()
        {
            this->p = TTree::algo::successor(this->p);
            return *this;
        }

        _tree_iterator operator++(int) throw()
        {
            _tree_iterator tmp = *this;
            this->p = TTree::algo::successor(this->p);
            return tmp;
        }

        _tree_iterator& operator--() throw()
        {
            this->p = TTree::algo::predecessor(this->p);
            return *this;
        }

        _tree_iterator operator--(int) throw()
        {
            _tree_iterator tmp = *this;
            this->p = TTree::algo::predecessor(this->p);
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

        typedef ft::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        typename TTree::node_type::pointer_type p;

        _tree_const_iterator() throw()
            : p() {}

        explicit _tree_const_iterator(typename TTree::node_type* p) throw()
            : p(p) {}

        _tree_const_iterator(const _tree_const_iterator& that) throw()
            : p(that.p) {}

        _tree_const_iterator(const _tree_iterator<T, TTree>& that) throw()
            : p(that.p) {}

        _tree_const_iterator& operator=(const _tree_const_iterator& that) throw()
        {
            this->p = that.p;
            return *this;
        }

        typename TTree::node_type* base() const throw()
        {
            return this->p;
        }

        reference operator*() const throw()
        {
            return static_cast<typename TTree::node_with_value_type::pointer_type>(this->p)->data;
        }

        pointer operator->() const throw()
        {
            return &static_cast<typename TTree::node_with_value_type::pointer_type>(this->p)->data;
        }

        _tree_const_iterator& operator++() throw()
        {
            this->p = TTree::algo::successor(const_cast<typename TTree::algo::node_pointer>(this->p));
            return *this;
        }

        _tree_const_iterator operator++(int) throw()
        {
            _tree_const_iterator tmp = *this;
            this->p = TTree::algo::successor(const_cast<typename TTree::algo::node_pointer>(this->p));
            return tmp;
        }

        _tree_const_iterator& operator--() throw()
        {
            this->p = TTree::algo::predecessor(const_cast<typename TTree::algo::node_pointer>(this->p));
            return *this;
        }

        _tree_const_iterator operator--(int) throw()
        {
            _tree_const_iterator tmp = *this;
            this->p = TTree::algo::predecessor(const_cast<typename TTree::algo::node_pointer>(this->p));
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

    // TKeySelector: const TKey& (*keySelector)(const T&)
    // TComp: bool (*comp)(const TKey&, const TKey&)
    template <typename TKey, typename T, typename TKeySelector, typename TComp = ft::less<TKey>, typename TAlloc = std::allocator<T> >
    class _tree
    {
    public:
        typedef TKey key_type;
        typedef T value_type;
        typedef _node node_type;
        typedef _tree_algorithm algo;
        typedef _node_with_data<T> node_with_value_type;
        typedef TKeySelector key_selector;
        typedef TComp key_compare;
        typedef typename TAlloc::template rebind<node_with_value_type>::other allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef _tree_iterator<T, _tree> iterator;
        typedef _tree_const_iterator<T, _tree> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> reverse_const_iterator;

    private:
        node_type header;

        key_compare comp;
        allocator_type alloc;
        size_type number;

    public:
        _tree(const TComp& comp = TComp(), const TAlloc& alloc = TAlloc())
            : header(), comp(comp), alloc(alloc), number()
        {
            this->reset();
        }

        _tree(const _tree& that)
            : header(), comp(that.comp), alloc(that.alloc), number(that.number)
        {
            this->copy(that.root_node());
        }

        ~_tree()
        {
            this->destruct(this->root_node());
        }

        _tree& operator=(const _tree& that)
        {
            if (this != &that)
            {
                _tree temp = that;
                this->swap(temp);
            }
            return *this;
        }

    public:
        allocator_type get_allocator() const { return this->alloc; }
        key_compare key_comp() const { return this->comp; }

    protected:
        node_type* insert_raw(node_type* parent, bool left, const value_type& data)
        {
            typename node_with_value_type::pointer_type node = this->alloc.allocate(1);
            try
            {
                this->alloc.construct(node, node_with_value_type(data));
            }
            catch (...)
            {
                this->alloc.deallocate(node, 1);
                throw;
            }

            node->parent = parent;
            if (parent == this->end_node())
            {
                this->header.parent = node;
                // update minimum
                this->header.left = node;
                // update maximum
                this->header.right = node;
            }
            else if (left)
            {
                parent->left = node;
                if (parent == this->begin_node())
                {
                    // update minimum
                    this->header.left = node;
                }
            }
            else // if (!left)
            {
                parent->right = node;
                if (parent == this->header.right)
                {
                    // update maximum
                    this->header.right = node;
                }
            }

            algo::repair_after_insert(this->header_node(), node);
            this->number++;

#ifdef FT_TREE_ASSERT
            this->validate();
#endif

            return node;
        }

        ft::pair<node_type*, node_type*> equal_range_raw(const TKey& lower_key, const TKey& upper_key) const
        {
            node_type* x = this->root_node();
            node_type* y = this->header_node();
            while (x != NULL)
            {
                if (this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(x)->data), lower_key))
                {
                    x = x->right;
                }
                else if (this->comp(upper_key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(x)->data)))
                {
                    y = x;
                    x = x->left;
                }
                else
                {
                    y = upper_bound_raw(x->right, y, upper_key);
                    x = lower_bound_raw(x->left, x, lower_key);
                    return ft::make_pair<node_type*, node_type*>(x, y);
                }
            }
            return ft::make_pair<node_type*, node_type*>(y, y);
        }

        node_type* lower_bound_raw(node_type* root, node_type* end, const TKey& key) const
        {
            node_type* it = root;
            node_type* result = end;
            while (it != NULL)
            {
                if (!this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(it)->data), key))
                {
                    result = it;
                    it = it->left;
                }
                else
                {
                    it = it->right;
                }
            }
            return result;
        }

        node_type* upper_bound_raw(node_type* root, node_type* end, const TKey& key) const
        {
            node_type* it = root;
            node_type* result = end;
            while (it != NULL)
            {
                if (this->comp(key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(it)->data)))
                {
                    result = it;
                    it = it->left;
                }
                else
                {
                    it = it->right;
                }
            }
            return result;
        }

        void reset()
        {
            this->header.left = this->end_node();
            this->header.right = this->end_node();
            this->header.parent = NULL;
        }

        void copy(node_type* source)
        {
            if (source == NULL)
            {
                this->reset();
                return;
            }

            typename node_with_value_type::pointer_type src_current = static_cast<typename node_with_value_type::pointer_type>(source);
            typename node_with_value_type::pointer_type dest_current = this->alloc.allocate(1);

            this->header.left = dest_current;
            this->header.right = dest_current;
            this->header.parent = dest_current;

            this->alloc.construct(dest_current, *src_current);

            dest_current->left = NULL;
            dest_current->right = NULL;
            dest_current->parent = this->header_node();

            for (;;)
            {
                if (src_current->left != NULL && dest_current->left == NULL)
                {
                    // Down left
                    src_current = static_cast<typename node_with_value_type::pointer_type>(src_current->left);
                    typename node_with_value_type::pointer_type node = this->alloc.allocate(1);

                    dest_current->left = node;

                    this->alloc.construct(node, *src_current);

                    node->left = NULL;
                    node->right = NULL;
                    node->parent = dest_current;

                    dest_current = node;
                    if (this->header.parent == this->header.right)
                    {
                        this->header.left = node;
                    }
                }
                else if (src_current->right != NULL && dest_current->right == NULL)
                {
                    // Down right
                    src_current = static_cast<typename node_with_value_type::pointer_type>(src_current->right);
                    typename node_with_value_type::pointer_type node = this->alloc.allocate(1);

                    dest_current->right = node;

                    this->alloc.construct(node, *src_current);

                    node->left = NULL;
                    node->right = NULL;
                    node->parent = dest_current;

                    dest_current = node;
                    this->header.right = node;
                }
                else if (src_current != source)
                {
                    // Up parent
                    src_current = static_cast<typename node_with_value_type::pointer_type>(src_current->parent);
                    dest_current = static_cast<typename node_with_value_type::pointer_type>(dest_current->parent);
                }
                else
                {
                    break;
                }
            }
        }

        void destruct(node_type* node)
        {
            while (node != NULL)
            {
                node_type* next = node->left;
                if (next != NULL)
                {
                    // if left is alive
                    // rotate_right_rough
                    node->left = next->right;
                    next->right = node;
                }
                else
                {
                    // trying dispose right
                    next = node->right;

                    typename node_with_value_type::pointer_type data_node = static_cast<typename node_with_value_type::pointer_type>(node);
                    this->alloc.destroy(data_node);
                    this->alloc.deallocate(data_node, 1);
                }
                node = next;
            }
        }

        node_type* header_node() { return &this->header; }
        node_type* header_node() const { return const_cast<node_type*>(&this->header); }
        node_type* root_node() { return this->header.parent; }
        node_type* root_node() const { return this->header.parent; }
        node_type* begin_node() { return this->header.left; }
        node_type* begin_node() const { return this->header.left; }
        node_type* end_node() { return this->header_node(); }
        node_type* end_node() const { return this->header_node(); }

#ifdef FT_TREE_ASSERT
        void validate()
        {
            assert(this->header.color == black);
            if (this->header.parent != NULL)
            {
                assert(this->header.parent->color == black);
            }

            (void)validate_internal(this->header.parent, std::size_t());
        }

        std::size_t validate_internal(node_type* node, std::size_t depth)
        {
            if (node == NULL)
            {
                return 1;
            }

            if (node->color == red)
            {
                assert(node->left == NULL || node->left->color == black);
                assert(node->right == NULL || node->right->color == black);
            }

            std::size_t black_count_left = validate_internal(node->left, depth + 1);
            std::size_t black_count_right = validate_internal(node->right, depth + 1);
            assert(black_count_left == black_count_right);

            std::size_t black_count = black_count_left;
            if (node->color == black)
            {
                black_count++;
            }
            return black_count;
        }
#endif

    public:
        iterator begin() { return iterator(this->begin_node()); }
        const_iterator begin() const { return const_iterator(this->begin_node()); }
        iterator end() { return iterator(this->end_node()); }
        const_iterator end() const { return const_iterator(this->end_node()); }
        reverse_iterator rbegin() { return reverse_iterator(this->end()); }
        reverse_const_iterator rbegin() const { return reverse_const_iterator(this->end()); }
        reverse_iterator rend() { return reverse_iterator(this->begin()); }
        reverse_const_iterator rend() const { return reverse_const_iterator(this->begin()); }

        bool empty() const { return this->root_node() == NULL; }
        size_type size() const { return this->number; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(node_type); }

        void clear()
        {
            this->destruct(this->root_node());
            this->reset();
            this->number = size_type();
        }

        ft::pair<node_type*, bool> insert_unique(node_type* hint, const value_type& data)
        {
            const key_type& key = key_selector()(data);
            node_type* parent = this->end_node();
            bool left = true;

            do
            {
                if (hint != NULL)
                {
                    if (hint == this->end_node() || this->comp(key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(hint)->data)))
                    {
                        node_type* hint_prev;
                        if (hint == this->begin_node())
                        {
                            hint_prev = NULL;
                        }
                        else
                        {
                            hint_prev = algo::predecessor(hint);
                        }
                        if (hint_prev == NULL || this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(hint_prev)->data), key))
                        {
                            left = this->root_node() == NULL || hint->left == NULL;
                            if (left)
                            {
                                parent = hint;
                            }
                            else
                            {
                                parent = hint_prev;
                            }
                            break;
                        }
                    }
                }

                node_type* it = this->root_node();
                node_type* result = this->header_node();
                while (it != NULL)
                {
                    parent = it;
                    left = this->comp(key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(it)->data));
                    if (left)
                    {
                        it = it->left;
                    }
                    else
                    {
                        result = it;
                        it = it->right;
                    }
                }
                if (result != this->end_node() && !this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(result)->data), key))
                {
                    return ft::make_pair(result, false);
                }
            } while (0);

            return ft::make_pair(this->insert_raw(parent, left, data), true);
        }

        node_type* insert(node_type* hint, const value_type& data)
        {
            const key_type& key = key_selector()(data);
            node_type* parent = this->end_node();
            bool left = true;

            do
            {
                if (hint != NULL)
                {
                    if (hint == this->end_node() || !this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(hint)->data), key))
                    {
                        node_type* hint_prev;
                        if (hint == this->begin_node())
                        {
                            hint_prev = NULL;
                        }
                        else
                        {
                            hint_prev = algo::predecessor(hint);
                        }
                        if (hint_prev == NULL || !this->comp(key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(hint_prev)->data)))
                        {
                            left = this->root_node() == NULL || hint->left == NULL;
                            if (left)
                            {
                                parent = hint;
                            }
                            else
                            {
                                parent = hint_prev;
                            }
                            break;
                        }
                        else
                        {
                            goto LABEL_INSERT_UPPER_BOUND;
                        }
                    }
                    else
                    {
                        goto LABEL_INSERT_LOWER_BOUND;
                    }
                }

            LABEL_INSERT_UPPER_BOUND:
            {
                node_type* it = this->root_node();
                while (it != NULL)
                {
                    parent = it;
                    if (this->comp(key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(it)->data)))
                    {
                        it = it->left;
                    }
                    else
                    {
                        it = it->right;
                    }
                }
                left = parent == this->end_node() || this->comp(key, key_selector()(static_cast<typename node_with_value_type::pointer_type>(parent)->data));
                break;
            }

            LABEL_INSERT_LOWER_BOUND:
            {
                node_type* it = this->root_node();
                while (it != NULL)
                {
                    parent = it;
                    if (!this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(it)->data), key))
                    {
                        it = it->left;
                    }
                    else
                    {
                        it = it->right;
                    }
                }
                left = parent == this->end_node() || !this->comp(key_selector()(static_cast<typename node_with_value_type::pointer_type>(parent)->data), key);
                break;
            }
            } while (0);

            return this->insert_raw(parent, left, data);
        }

        void erase(node_type* z)
        {
            node_type* y;
            node_type* x;

            node_type* z_left = z->left;
            node_type* z_right = z->right;

            if (z_left == NULL)
            {
                y = z;
                // maybe null
                x = z_right;
            }
            else if (z_right == NULL)
            {
                y = z;
                // not null
                x = z_left;
            }
            else
            {
                // assert(algo::successor(z) == algo::minimum(z_right));
                y = algo::minimum(z_right);
                // maybe null
                x = y->right;
                // goto LABEL_COMPLETE_NODE;
            }

            node_type* x_parent;
            if (y == z)
            {
                // assert(z_left == NULL || z_right == NULL);
                if (x != NULL)
                {
                    x->parent = z->parent;
                }
                x_parent = z->parent;
                algo::set_child(z->parent, z == z->parent->left, x, this->header_node());

                if (this->begin_node() == z)
                {
                    // update minimum
                    // assert(z_left == NULL);
                    if (z_right != NULL)
                    {
                        z_left = algo::minimum(z_right);
                    }
                    else
                    {
                        z_left = z->parent;
                    }
                    this->header.left = z_left;
                    z_left = NULL;
                }
                if (this->header.right == z)
                {
                    // update maximum
                    // assert(z_right == NULL);
                    if (z_left != NULL)
                    {
                        z_right = algo::maximum(z_left);
                    }
                    else
                    {
                        z_right = z->parent;
                    }
                    this->header.right = z_right;
                    z_right = NULL;
                }
            }
            else
            {
                // LABEL_COMPLETE_NODE:
                z_left->parent = y;
                y->left = z_left;
                if (y == z_right)
                {
                    x_parent = y;
                    // assert(x == NULL)
                }
                else
                {
                    y->right = z_right;
                    z_right->parent = y;

                    x_parent = y->parent;
                    // assert(x_parent->left == y);

                    if (x != NULL)
                    {
                        x->parent = x_parent;
                    }
                    x_parent->left = x;
                }

                y->parent = z->parent;
                algo::set_child(z->parent, z == z->parent->left, y, this->header_node());
            }

            algo::repair_after_erase(this->header_node(), z, y, x, x_parent);

            typename node_with_value_type::pointer_type data_z = static_cast<typename node_with_value_type::pointer_type>(z);
            this->alloc.destroy(data_z);
            this->alloc.deallocate(data_z, 1);
            this->number--;

#ifdef FT_TREE_ASSERT
            this->validate();
#endif
        }

        void swap(_tree& that)
        {
            ft::swap(this->header.left, that.header.left);
            if (this->header.left == &that.header)
            {
                this->header.left = &this->header;
            }
            if (that.header.left == &this->header)
            {
                that.header.left = &that.header;
            }

            ft::swap(this->header.right, that.header.right);
            if (this->header.right == &that.header)
            {
                this->header.right = &this->header;
            }
            if (that.header.right == &this->header)
            {
                that.header.right = &that.header;
            }

            ft::swap(this->header.parent, that.header.parent);
            if (this->header.parent != NULL)
            {
                this->header.parent->parent = &this->header;
            }
            if (that.header.parent != NULL)
            {
                that.header.parent->parent = &that.header;
            }

            ft::swap(this->comp, that.comp);
            ft::swap(this->alloc, that.alloc);
            ft::swap(this->number, that.number);
        }

        size_type count(const key_type& key) const
        {
            ft::pair<const_iterator, const_iterator> range = this->equal_range(key);
            size_type i = size_type();
            for (const_iterator it = range.first; it != range.second; ++it)
            {
                i++;
            }
            return i;
        }

        node_type* find(const key_type& key) { return const_cast<const _tree*>(this)->find(key); }
        node_type* find(const key_type& key) const
        {
            const_iterator it = this->lower_bound(key);
            if (it == this->end() || this->comp(key, key_selector()(*it)))
            {
                return this->end_node();
            }
            return it.base();
        }

        ft::pair<iterator, iterator> equal_range(const key_type& key)
        {
            ft::pair<node_type*, node_type*> range = const_cast<const _tree*>(this)->equal_range_raw(key, key);
            return ft::make_pair(iterator(range.first), iterator(range.second));
        }
        ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
        {
            ft::pair<node_type*, node_type*> range = this->equal_range_raw(key, key);
            return ft::make_pair(const_iterator(range.first), const_iterator(range.second));
        }

        iterator lower_bound(const key_type& key)
        {
            return iterator(const_cast<const _tree*>(this)->lower_bound_raw(this->root_node(), this->end_node(), key));
        }
        const_iterator lower_bound(const key_type& key) const
        {
            return const_iterator(this->lower_bound_raw(this->root_node(), this->end_node(), key));
        }

        iterator upper_bound(const key_type& key)
        {
            return iterator(const_cast<const _tree*>(this)->upper_bound_raw(this->root_node(), this->end_node(), key));
        }
        const_iterator upper_bound(const key_type& key) const
        {
            return const_iterator(this->upper_bound_raw(this->root_node(), this->end_node(), key));
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
