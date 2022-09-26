/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <_tree.hpp>
#include <utility.hpp>

#include <cstddef>
#include <functional>
#include <memory>

namespace ft
{
    template <typename T>
    struct _select_self
    {
        const T& operator()(const T& t) const
        {
            return t;
        }
    };

    template <typename T, typename TComp = std::less<T>, typename TAlloc = std::allocator<T> >
    class set
    {
    public:
        typedef T key_type;
        typedef T value_type;
        typedef TComp key_compare;
        typedef TComp value_compare;
        typedef TAlloc allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

    protected:
        typedef _select_self<value_type> key_select;
        typedef ft::_tree<key_type, value_type, key_select, key_compare, allocator_type> container_type;

    public:
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename container_type::iterator iterator;
        typedef typename container_type::const_iterator const_iterator;
        typedef typename ft::reverse_iterator<iterator> reverse_iterator;
        typedef typename ft::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        container_type c;
        allocator_type alloc;

    public:
        set();
        explicit set(const key_compare& comp, const allocator_type& alloc = allocator_type());
        template <class UIter>
        set(UIter first, UIter last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
        set(const set& that);
        ~set();
        set& operator=(const set& that);
        allocator_type get_allocator() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;

        bool empty() const;
        size_type size() const;
        size_type max_size() const;

        void clear();
        ft::pair<iterator, bool> insert(const value_type& value);
        template <class UIter>
        void insert(UIter first, UIter last);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        size_type erase(const key_type& key);
        void swap(set& that);

        size_type count(const key_type& key) const;
        iterator find(const key_type& key);
        const_iterator find(const key_type& key) const;
        ft::pair<iterator, iterator> equal_range(const key_type& key);
        ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;
        iterator lower_bound(const key_type& key);
        const_iterator lower_bound(const key_type& key) const;
        iterator upper_bound(const key_type& key);
        const_iterator upper_bound(const key_type& key) const;

        key_compare key_comp() const;
        value_compare value_comp() const;
    };

    template <typename T, typename TComp, typename TAlloc>
    inline bool operator==(
        const set<T, TComp, TAlloc>& lhs,
        const set<T, TComp, TAlloc>& rhs);

    template <typename T, typename TComp, typename TAlloc>
    inline bool operator!=(
        const set<T, TComp, TAlloc>& lhs,
        const set<T, TComp, TAlloc>& rhs);

    template <typename T, typename TComp, typename TAlloc>
    inline bool operator<(
        const set<T, TComp, TAlloc>& lhs,
        const set<T, TComp, TAlloc>& rhs);

    template <typename T, typename TComp, typename TAlloc>
    inline bool operator<=(
        const set<T, TComp, TAlloc>& lhs,
        const set<T, TComp, TAlloc>& rhs);

    template <typename T, typename TComp, typename TAlloc>
    inline bool operator>(
        const set<T, TComp, TAlloc>& lhs,
        const set<T, TComp, TAlloc>& rhs);

    template <typename T, typename TComp, typename TAlloc>
    inline bool operator>=(
        const set<T, TComp, TAlloc>& lhs,
        const set<T, TComp, TAlloc>& rhs);

    template <typename T, typename TComp, typename TAlloc>
    inline void swap(
        set<T, TComp, TAlloc>& lhs,
        set<T, TComp, TAlloc>& rhs);
}
