/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "algorithm.hpp"
#include "functional.hpp"
#include "iterator.hpp"
#include "stdexcept.hpp"
#include "type_traits.hpp"

#include <cstddef>
#include <limits>

namespace ft
{
    struct _list_node_base
    {
        typedef _list_node_base* pointer_type;

        pointer_type prev, next;

        _list_node_base()
            : prev(), next() {}

        _list_node_base(const _list_node_base& that)
            : prev(that.prev), next(that.next) {}

        ~_list_node_base() {}

        _list_node_base& operator=(const _list_node_base& that)
        {
            this->prev = that.prev;
            this->next = that.next;
            return *this;
        }
    };

    template <typename T>
    struct _list_node : _list_node_base
    {
        T data;

        explicit _list_node(const T& data)
            : _list_node_base(), data(data) {}

        _list_node(const _list_node& that)
            : _list_node_base(that), data(that.data) {}

        ~_list_node() {}

        _list_node& operator=(const _list_node& that)
        {
            this->_list_node_base::operator=(that);
            this->data = that.data;
            return *this;
        }
    };

    template <typename TList>
    struct _list_iterator
    {
        typedef typename TList::value_type value_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        typedef ft::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        _list_node_base::pointer_type p;

        _list_iterator() throw()
            : p() {}

        explicit _list_iterator(_list_node_base::pointer_type p) throw()
            : p(p) {}

        _list_iterator(const _list_iterator& that) throw()
            : p(that.p) {}

        _list_iterator& operator=(const _list_iterator& that) throw()
        {
            this->p = that.p;
            return *this;
        }

        _list_node_base::pointer_type base() const throw()
        {
            return this->p;
        }

        reference operator*() const throw()
        {
            return static_cast<_list_node<typename TList::value_type>*>(this->p)->data;
        }

        pointer operator->() const throw()
        {
            return &static_cast<_list_node<typename TList::value_type>*>(this->p)->data;
        }

        _list_iterator& operator++() throw()
        {
            this->p = this->p->next;
            return *this;
        }

        _list_iterator operator++(int) throw()
        {
            _list_iterator tmp = *this;
            this->p = this->p->next;
            return tmp;
        }

        _list_iterator& operator--() throw()
        {
            this->p = this->p->prev;
            return *this;
        }

        _list_iterator operator--(int) throw()
        {
            _list_iterator tmp = *this;
            this->p = this->p->prev;
            return tmp;
        }

        friend bool operator==(const _list_iterator& lhs, const _list_iterator& rhs) throw()
        {
            return lhs.p == rhs.p;
        }

        friend bool operator!=(const _list_iterator& lhs, const _list_iterator& rhs) throw()
        {
            return lhs.p != rhs.p;
        }
    };

    template <typename TList>
    struct _list_const_iterator
    {
        typedef const typename TList::value_type value_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        typedef ft::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        _list_node_base::pointer_type p;

        _list_const_iterator() throw()
            : p() {}

        explicit _list_const_iterator(_list_node_base::pointer_type p) throw()
            : p(p) {}

        _list_const_iterator(const _list_const_iterator& that) throw()
            : p(that.p) {}

        _list_const_iterator(const _list_iterator<TList>& that) throw()
            : p(that.p) {}

        _list_const_iterator& operator=(const _list_const_iterator& that) throw()
        {
            this->p = that.p;
            return *this;
        }

        _list_node_base::pointer_type base() const throw()
        {
            return this->p;
        }

        reference operator*() const throw()
        {
            return static_cast<_list_node<typename TList::value_type>*>(this->p)->data;
        }

        pointer operator->() const throw()
        {
            return &static_cast<_list_node<typename TList::value_type>*>(this->p)->data;
        }

        _list_const_iterator& operator++() throw()
        {
            this->p = this->p->next;
            return *this;
        }

        _list_const_iterator operator++(int) throw()
        {
            _list_const_iterator tmp = *this;
            this->p = this->p->next;
            return tmp;
        }

        _list_const_iterator& operator--() throw()
        {
            this->p = this->p->prev;
            return *this;
        }

        _list_const_iterator operator--(int) throw()
        {
            _list_const_iterator tmp = *this;
            this->p = this->p->prev;
            return tmp;
        }

        friend bool operator==(const _list_const_iterator& lhs, const _list_const_iterator& rhs) throw()
        {
            return lhs.p == rhs.p;
        }

        friend bool operator!=(const _list_const_iterator& lhs, const _list_const_iterator& rhs) throw()
        {
            return lhs.p != rhs.p;
        }
    };

    template <typename T, typename TAlloc = std::allocator<T> >
    class list
    {
    public:
        typedef T value_type;
        typedef TAlloc allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef _list_iterator<list> iterator;
        typedef _list_const_iterator<list> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

    protected:
        typedef _list_node<value_type> node_type;
        typedef typename TAlloc::template rebind<node_type>::other node_allocator_type;

    private:
        _list_node_base header;

        node_allocator_type alloc;
        size_type number;

    public:
        list()
            : header(), alloc(), number()
        {
            this->reset();
        }

        explicit list(const allocator_type& alloc)
            : header(), alloc(alloc), number()
        {
            this->reset();
        }

        explicit list(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type())
            : header(), alloc(alloc), number()
        {
            this->reset();
            this->assign(count, value);
        }

        template <typename UIter>
        // list(UIter first, UIter last, const allocator_type& alloc = allocator_type())
        list(typename ft::enable_if<ft::is_iterator<UIter>::value, UIter>::type first, UIter last, const allocator_type& alloc = allocator_type())
            : header(), alloc(alloc), number()
        {
            this->reset();
            this->assign(first, last);
        }

        list(const list& that)
            : header(), alloc(that.alloc), number()
        {
            this->reset();
            this->assign(that.begin(), that.end());
        }

        ~list()
        {
            static_cast<void>(this->destruct(this->header.next, &this->header));
        }

        list& operator=(const list& that)
        {
            if (this != &that)
            {
                this->assign(that.begin(), that.end());
            }
            return *this;
        }

    protected:
        size_type destruct(_list_node_base::pointer_type node, _list_node_base::pointer_type node_end)
        {
            size_type distance = 0;
            while (node != node_end)
            {
                _list_node_base::pointer_type next = node->next;

                node_type* data_node = static_cast<node_type*>(node);
                this->alloc.destroy(data_node);
                this->alloc.deallocate(data_node, 1);
                distance++;

                node = next;
            }
            return distance;
        }

        void reset()
        {
            this->header.prev = &this->header;
            this->header.next = &this->header;
        }

        node_type* create_node(const value_type& value)
        {
            node_type* node = this->alloc.allocate(1);
            try
            {
                this->alloc.construct(node, node_type(value));
                return node;
            }
            catch (...)
            {
                this->alloc.deallocate(node, 1);
                throw;
            }
        }

    public:
        void assign(size_type count, const value_type& value)
        {
            iterator it = this->begin();
            iterator end = this->end();
            for (; it != end && count != 0; --count)
            {
                *it = value;
                ++it;
            }
            if (it != end)
            {
                this->erase(it, end);
            }
            else if (count != 0)
            {
                this->insert(it, count, value);
            }
        }

        template <typename UIter>
        void assign(UIter first, UIter last)
        {
            iterator it = this->begin();
            iterator end = this->end();
            for (; it != end && first != last; ++first)
            {
                *it = *first;
                ++it;
            }
            if (it != end)
            {
                this->erase(it, end);
            }
            else if (first != last)
            {
                this->insert(it, first, last);
            }
        }

    public:
        allocator_type get_allocator() const { return this->alloc; }

    public:
        reference front() { return static_cast<node_type*>(this->header.next)->data; }
        const_reference front() const { return static_cast<node_type*>(this->header.next)->data; }
        reference back() { return static_cast<node_type*>(this->header.prev)->data; }
        const_reference back() const { return static_cast<node_type*>(this->header.prev)->data; }

    public:
        iterator begin() { return iterator(this->header.next); }
        const_iterator begin() const { return const_iterator(this->header.next); }
        iterator end() { return iterator(&this->header); }
        const_iterator end() const { return const_iterator(const_cast<_list_node_base*>(&this->header)); }
        reverse_iterator rbegin() { return reverse_iterator(this->end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }
        reverse_iterator rend() { return reverse_iterator(this->begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

    public:
        bool empty() const { return this->size() == size_type(); }
        size_type size() const { return this->number; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(node_type); }

    protected:
        void link(_list_node_base::pointer_type pos, _list_node_base::pointer_type head, _list_node_base::pointer_type tail)
        {
            _list_node_base::pointer_type prev = pos->prev;
            _list_node_base::pointer_type next = pos;
            prev->next = head;
            head->prev = prev;
            tail->next = next;
            next->prev = tail;
        }

        void unlink(_list_node_base::pointer_type head, _list_node_base::pointer_type tail)
        {
            _list_node_base::pointer_type prev = head->prev;
            _list_node_base::pointer_type next = tail->next;
            prev->next = next;
            head->prev = _list_node_base::pointer_type();
            tail->next = _list_node_base::pointer_type();
            next->prev = prev;
        }

    public:
        void clear()
        {
            static_cast<void>(this->destruct(this->header.next, &this->header));
            this->reset();
            this->number = size_type();
        }

        iterator insert(iterator pos, const value_type& value)
        {
            node_type* node = this->create_node(value);
            this->link(pos.base(), node, node);
            this->number++;
            return iterator(node);
        }

        iterator insert(iterator pos, size_type count, const value_type& value)
        {
            if (count == 0)
            {
                return pos;
            }

            size_type distance = 0;
            node_type* head = this->create_node(value);
            node_type* tail = head;
            --count;
            distance++;
            try
            {
                for (; count != 0; --count)
                {
                    node_type* node = this->create_node(value);

                    tail->next = node;
                    node->prev = tail;
                    tail = node;

                    distance++;
                }
            }
            catch (...)
            {
                static_cast<void>(this->destruct(head, _list_node_base::pointer_type()));
                throw;
            }
            this->link(pos.base(), head, tail);
            this->number += distance;
            return iterator(head);
        }

        template <typename UIter>
        iterator insert(iterator pos, UIter first, UIter last)
        {
            if (first == last)
            {
                return pos;
            }

            size_type distance = 0;
            node_type* head = this->create_node(*first);
            node_type* tail = head;
            ++first;
            distance++;
            try
            {
                for (; first != last; ++first)
                {
                    node_type* node = this->create_node(*first);

                    tail->next = node;
                    node->prev = tail;
                    tail = node;

                    distance++;
                }
            }
            catch (...)
            {
                static_cast<void>(this->destruct(head, _list_node_base::pointer_type()));
                throw;
            }
            this->link(pos.base(), head, tail);
            this->number += distance;
            return iterator(head);
        }

        iterator erase(iterator pos)
        {
            _list_node_base::pointer_type node = pos.base();
            ++pos;
            this->unlink(node, node);
            this->number -= this->destruct(node, _list_node_base::pointer_type());
            return pos;
        }

        iterator erase(iterator first, iterator last)
        {
            if (first == last)
            {
                return first;
            }

            iterator tail = last;
            --tail;

            _list_node_base::pointer_type head = first.base();
            this->unlink(head, tail.base());
            this->number -= this->destruct(head, _list_node_base::pointer_type());
            return last;
        }

        void push_back(const value_type& value)
        {
            static_cast<void>(this->insert(this->end(), value));
        }

        void pop_back()
        {
            static_cast<void>(this->erase(iterator(this->header.prev)));
        }

        void push_front(const value_type& value)
        {
            static_cast<void>(this->insert(this->begin(), value));
        }

        void pop_front()
        {
            static_cast<void>(this->erase(this->begin()));
        }

        void resize(size_type count, value_type value = value_type())
        {
            size_type size = this->size();
            if (count > size)
            {
                this->insert(this->end(), count - size, value);
            }
            else if (count < size)
            {
                iterator it = this->begin();
                iterator end = this->end();
                ft::advance(it, count);
                this->erase(it, end);
            }
        }

        void swap(list& that)
        {
            ft::swap(this->header.next, that.header.next);
            if (this->header.next == &that.header)
            {
                this->header.next = &this->header;
            }
            else
            {
                this->header.next->prev = &this->header;
            }
            if (that.header.next == &this->header)
            {
                that.header.next = &that.header;
            }
            else
            {
                that.header.next->prev = &that.header;
            }

            ft::swap(this->header.prev, that.header.prev);
            if (this->header.prev == &that.header)
            {
                this->header.prev = &this->header;
            }
            else
            {
                this->header.prev->next = &this->header;
            }
            if (that.header.prev == &this->header)
            {
                that.header.prev = &that.header;
            }
            else
            {
                that.header.prev->next = &that.header;
            }

            ft::swap(this->number, that.number);

            ft::swap(this->alloc, that.alloc);
        }

    public:
        void merge(list& that)
        {
            this->merge(that, ft::less<value_type>());
        }

        template <typename TComp>
        void merge(list& that, TComp comp)
        {
            if (this == &that)
            {
                return;
            }

            iterator b1 = this->begin();
            iterator e1 = this->end();
            iterator b2 = that.begin();
            iterator e2 = that.end();
            while (b1 != e1 && b2 != e2)
            {
                if (comp(*b2, *b1))
                {
                    size_type distance = size_type();
                    iterator it2 = b2;
                    do
                    {
                        ++distance;
                        ++it2;
                    } while (it2 != e2 && comp(*it2, *b1));

                    this->number += distance;
                    that.number -= distance;

                    _list_node_base::pointer_type head = b2.base();
                    _list_node_base::pointer_type tail = it2.base()->prev;

                    b2 = it2; // tail->next
                    that.unlink(head, tail);

                    iterator it1 = b1;
                    ++it1;
                    this->link(b1.base(), head, tail);
                    b1 = it1; // tail->next
                }
                else
                {
                    ++b1;
                }
            }
            this->splice(e1, that);
        }

        void splice(const_iterator pos, list& that)
        {
            if (this == &that)
            {
                // NOTE: Undefined behavior
                return;
            }

            _list_node_base::pointer_type head = that.header.next;
            if (head != &that.header)
            {
                _list_node_base::pointer_type tail = that.header.prev;
                this->number += that.number;
                that.number = 0;
                that.unlink(head, tail);
                this->link(pos.base(), head, tail);
            }
        }

        void splice(const_iterator pos, list& that, const_iterator it)
        {
            if (pos != it)
            {
                _list_node_base::pointer_type node = it.base();
                if (this != &that)
                {
                    this->number++;
                    that.number--;
                }
                that.unlink(node, node);
                this->link(pos.base(), node, node);
            }
        }

        void splice(const_iterator pos, list& that, const_iterator first, const_iterator last)
        {
            if (first != last)
            {
                const_iterator tail = last;
                --tail;

                _list_node_base::pointer_type head = first.base();
                if (this != &that)
                {
                    difference_type distance = ft::distance(first, last);
                    this->number += distance;
                    that.number -= distance;
                }
                that.unlink(head, tail.base());
                this->link(pos.base(), head, tail.base());
            }
        }

        void remove(const value_type& value)
        {
            list garbages(this->get_allocator());
            for (const_iterator it = this->begin(), end = this->end(); it != end;)
            {
                if (*it == value)
                {
                    const_iterator it1 = it;
                    ++it1;
                    for (; it1 != end && *it1 == value; ++it1)
                        ;
                    garbages.splice(garbages.end(), *this, it, it1);
                    it = it1;
                    if (it != end)
                    {
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
        }

        template <typename TPredicate>
        void remove_if(TPredicate p)
        {
            list garbages(this->get_allocator());
            for (const_iterator it = this->begin(), end = this->end(); it != end;)
            {
                if (p(*it))
                {
                    const_iterator it1 = it;
                    ++it1;
                    for (; it1 != end && p(*it1); ++it1)
                        ;
                    garbages.splice(garbages.end(), *this, it, it1);
                    it = it1;
                    if (it != end)
                    {
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
        }

        void reverse()
        {
            if (this->number > 1)
            {
                for (_list_node_base::pointer_type it = this->header.next; it != &this->header; it = it->prev)
                {
                    ft::swap(it->prev, it->next);
                }
                ft::swap(this->header.prev, this->header.next);
            }
        }

        void unique()
        {
            this->unique(ft::equal_to<value_type>());
        }

        template <typename TBiPredicate>
        void unique(TBiPredicate p)
        {
            list garbages(this->get_allocator());
            for (iterator it = this->begin(), end = this->end(); it != end;)
            {
                iterator it1 = it;
                ++it1;
                for (; it1 != end && p(*it, *it1); ++it1)
                    ;
                if (++it != it1)
                {
                    garbages.splice(garbages.end(), *this, it, it1);
                    it = it1;
                }
            }
        }

        void sort()
        {
            this->sort(ft::less<value_type>());
        }

        template <typename TComp>
        void sort(TComp comp)
        {
            this->sort_internal(this->begin(), this->end(), this->size(), comp);
        }

    protected:
        template <class TComp>
        typename list::iterator sort_internal(iterator f1, iterator e2, size_type n, TComp& comp)
        {
            switch (n)
            {
            case 0:
            case 1:
                return f1;
            case 2:
                if (comp(*--e2, *f1))
                {
                    _list_node_base::pointer_type f = e2.base();
                    this->unlink(f, f);
                    this->link(f1.base(), f, f);
                    return e2;
                }
                return f1;
            }
            size_type n2 = n / 2;
            iterator e1 = f1;
            ft::advance(e1, n2);
            iterator r = f1 = this->sort_internal(f1, e1, n2, comp);
            iterator f2 = e1 = this->sort_internal(e1, e2, n - n2, comp);
            if (comp(*f2, *f1))
            {
                iterator m2 = f2;
                ++m2;
                for (; m2 != e2 && comp(*m2, *f1); ++m2)
                    ;
                _list_node_base::pointer_type f = f2.base();
                _list_node_base::pointer_type l = m2.base()->prev;
                r = f2;
                e1 = f2 = m2;
                this->unlink(f, l);
                m2 = f1;
                ++m2;
                this->link(f1.base(), f, l);
                f1 = m2;
            }
            else
                ++f1;
            while (f1 != e1 && f2 != e2)
            {
                if (comp(*f2, *f1))
                {
                    iterator m2 = f2;
                    ++m2;
                    for (; m2 != e2 && comp(*m2, *f1); ++m2)
                        ;
                    _list_node_base::pointer_type f = f2.base();
                    _list_node_base::pointer_type l = m2.base()->prev;
                    if (e1 == f2)
                        e1 = m2;
                    f2 = m2;
                    this->unlink(f, l);
                    m2 = f1;
                    ++m2;
                    this->link(f1.base(), f, l);
                    f1 = m2;
                }
                else
                    ++f1;
            }
            return r;
        }

    public:
        friend bool operator==(const list& lhs, const list& rhs)
        {
            return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        friend bool operator!=(const list& lhs, const list& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const list& lhs, const list& rhs)
        {
            return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

        friend bool operator<=(const list& lhs, const list& rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>(const list& lhs, const list& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator>=(const list& lhs, const list& rhs)
        {
            return !(lhs < rhs);
        }
    };

    template <typename T, typename TAlloc>
    inline void swap(
        list<T, TAlloc>& lhs,
        list<T, TAlloc>& rhs)
    {
        lhs.swap(rhs);
    }
}

namespace std
{
    template <typename T, typename TAlloc>
    inline void swap(
        ft::list<T, TAlloc>& lhs,
        ft::list<T, TAlloc>& rhs)
    {
        ft::swap(lhs, rhs);
    }
}
