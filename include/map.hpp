/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <_tree.hpp>
#include <utility.hpp>

#include <cstddef>
#include <exception>
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
            friend class map;

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
                return this->comp(key_select()(lhs), key_select()(rhs));
            }
        };

    private:
        container_type c;

    public:
        map()
            : c() {}

        explicit map(const key_compare& comp, const allocator_type& alloc = allocator_type())
            : c(comp, alloc) {}

        template <typename UIter>
        // map(UIter first, UIter last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
        map(typename ft::enable_if<ft::is_iterator<UIter>::value, UIter>::type first, UIter last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : c(comp, alloc)
        {
            this->insert(first, last);
        }

        map(const map& that)
            : c(that.c) {}

        ~map() {}

        map& operator=(const map& that)
        {
            this->c = that.c;
            return *this;
        }

    public:
        allocator_type get_allocator() const { return allocator_type(this->c.get_allocator()); }

    public:
        mapped_type& at(const key_type& key)
        {
            iterator it = this->find(key);
            if (it == this->end())
            {
                throw std::out_of_range("map::at");
            }
            return it->second;
        }
        const mapped_type& at(const key_type& key) const
        {
            const_iterator it = this->find(key);
            if (it == this->end())
            {
                throw std::out_of_range("map::at");
            }
            return it->second;
        }

        mapped_type& operator[](const key_type& key)
        {
            iterator it = this->find(key);
            if (it == this->end())
            {
                it = this->insert(ft::make_pair(key, mapped_type())).first;
            }
            return it->second;
        }

    public:
        iterator begin() { return this->c.begin(); }
        const_iterator begin() const { return this->c.begin(); }
        iterator end() { return this->c.end(); }
        const_iterator end() const { return this->c.end(); }
        reverse_iterator rbegin() { return this->c.rbegin(); }
        const_reverse_iterator rbegin() const { return this->c.rbegin(); }
        reverse_iterator rend() { return this->c.rend(); }
        const_reverse_iterator rend() const { return this->c.rend(); }

    public:
        bool empty() const { return this->c.empty(); }
        size_type size() const { return this->c.size(); }
        size_type max_size() const { return this->c.max_size(); }

    public:
        void clear() { return this->c.clear(); }

        ft::pair<iterator, bool> insert(const value_type& value)
        {
            ft::pair<typename container_type::node_type*, bool> result = this->c.insert_unique(NULL, value);
            return ft::make_pair(iterator(result.first), result.second);
        }

        iterator insert(iterator hint, const value_type& value)
        {
            return iterator(this->c.insert_unique(hint.base(), value).first);
        }

        template <typename UIter>
        // void insert(UIter first, UIter last)
        typename ft::enable_if<ft::is_iterator<UIter>::value, void>::type insert(UIter first, UIter last)
        {
            for (UIter it = first; it != last; ++it)
            {
                this->c.insert_unique(NULL, *it);
            }
        }

        iterator erase(iterator pos)
        {
            iterator it = pos++;
            this->c.erase(it.base());
            return pos;
        }

        iterator erase(iterator first, iterator last)
        {
            while (first != last)
            {
                iterator it = first++;
                this->c.erase(it.base());
            }
            return last;
        }

        size_type erase(const key_type& key)
        {
            iterator it = this->find(key);
            if (it == this->end())
            {
                return 0;
            }
            this->erase(it);
            return 1;
        }

        void swap(map& that) { this->c.swap(that.c); }

    public:
        size_type count(const key_type& key) const { return this->c.count(key); }

        iterator find(const key_type& key) { return iterator(this->c.find(key)); }
        const_iterator find(const key_type& key) const { return const_iterator(this->c.find(key)); }

        ft::pair<iterator, iterator> equal_range(const key_type& key) { return this->c.equal_range(key); }
        ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const { return this->c.equal_range(key); }

        iterator lower_bound(const key_type& key) { return this->c.lower_bound(key); }
        const_iterator lower_bound(const key_type& key) const { return this->c.lower_bound(key); }

        iterator upper_bound(const key_type& key) { return this->c.upper_bound(key); }
        const_iterator upper_bound(const key_type& key) const { return this->c.upper_bound(key); }

    public:
        key_compare key_comp() const { return this->c.key_comp(); }
        value_compare value_comp() const { return value_compare(this->c.key_comp()); }

    public:
        friend bool operator==(const map& lhs, const map& rhs)
        {
            return lhs.c == rhs.c;
        }

        friend bool operator!=(const map& lhs, const map& rhs)
        {
            return lhs.c != rhs.c;
        }

        friend bool operator<(const map& lhs, const map& rhs)
        {
            return lhs.c < rhs.c;
        }

        friend bool operator<=(const map& lhs, const map& rhs)
        {
            return lhs.c <= rhs.c;
        }

        friend bool operator>(const map& lhs, const map& rhs)
        {
            return lhs.c > rhs.c;
        }

        friend bool operator>=(const map& lhs, const map& rhs)
        {
            return lhs.c >= rhs.c;
        }
    };

    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline void swap(
        map<TKey, TMapped, TComp, TAlloc>& lhs,
        map<TKey, TMapped, TComp, TAlloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

namespace std
{
    template <typename TKey, typename TMapped, typename TComp, typename TAlloc>
    inline void swap(
        ft::map<TKey, TMapped, TComp, TAlloc>& lhs,
        ft::map<TKey, TMapped, TComp, TAlloc>& rhs)
    {
        ft::swap(lhs, rhs);
    }
}
