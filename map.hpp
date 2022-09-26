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
    struct _select_first
    {
        typename T::first_type operator()(const T& t) const
        {
            return t.first;
        }
    };

    template <typename TKey, typename TMapped, typename TComp = std::less<TKey>, typename TAlloc = std::allocator<ft::pair<const TKey, TMapped> > >
    class map
    {
    public:
        typedef TKey key_type;
        typedef TMapped mapped_type;
        typedef ft::pair<const TKey, TMapped> value_type;
        typedef TComp key_compare;
        typedef TAlloc allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

    protected:
        typedef _select_first<value_type> key_select;
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

        class value_compare
        {
        public:
            typedef bool result_type;
            typedef value_type first_argument_type;
            typedef value_type second_argument_type;

        protected:
            key_compare comp;

            value_compare(const key_compare& comp)
                : comp(comp) {}

        public:
            result_type operator()(const first_argument_type& lhs, const second_argument_type& rhs)
            {
                return this->comp(key_select()(lhs), key_select()(rhs.first));
            }
        };

    private:
        container_type c;
        allocator_type alloc;

    public:
        map();
        explicit map(const key_compare& comp, const allocator_type& alloc = allocator_type());
        template <class UIter>
        map(UIter first, UIter last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
        map(const map& that);
        ~map();
        map& operator=(const map& that);
        allocator_type get_allocator() const;

        reference at(const key_type& key);
        const_reference at(const key_type& key) const;
        reference operator[](const key_type& key);

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
        void swap(map& that);

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

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline bool operator==(
        const map<TKey, TMapped, TComp, TAlloc>& lhs,
        const map<TKey, TMapped, TComp, TAlloc>& rhs);

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline bool operator!=(
        const map<TKey, TMapped, TComp, TAlloc>& lhs,
        const map<TKey, TMapped, TComp, TAlloc>& rhs);

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline bool operator<(
        const map<TKey, TMapped, TComp, TAlloc>& lhs,
        const map<TKey, TMapped, TComp, TAlloc>& rhs);

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline bool operator<=(
        const map<TKey, TMapped, TComp, TAlloc>& lhs,
        const map<TKey, TMapped, TComp, TAlloc>& rhs);

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline bool operator>(
        const map<TKey, TMapped, TComp, TAlloc>& lhs,
        const map<TKey, TMapped, TComp, TAlloc>& rhs);

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline bool operator>=(
        const map<TKey, TMapped, TComp, TAlloc>& lhs,
        const map<TKey, TMapped, TComp, TAlloc>& rhs);

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline void swap(
        map<TKey, TMapped, TComp, TAlloc>& lhs,
        map<TKey, TMapped, TComp, TAlloc>& rhs);
}
