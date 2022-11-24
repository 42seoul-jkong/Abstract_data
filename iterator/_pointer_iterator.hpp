/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator_traits.hpp"

#include <memory.hpp>
#include <type_traits.hpp>

#include <cstddef>

namespace ft
{
    template <typename TIter, typename _TCont>
    struct _pointer_iterator
    {
    protected:
        typedef iterator_traits<TIter> _Traits;

    public:
        typedef TIter iterator_type;
        typedef typename _Traits::iterator_category iterator_category;
        typedef typename _Traits::value_type value_type;
        typedef typename _Traits::difference_type difference_type;
        typedef typename _Traits::pointer pointer;
        typedef typename _Traits::reference reference;

    protected:
        TIter current;

    public:
        _pointer_iterator() : current(iterator_type()) {}
        explicit _pointer_iterator(const iterator_type& current) : current(current) {}
        template <class U>
        _pointer_iterator(const _pointer_iterator<U, typename ft::enable_if<ft::is_same<U, typename _TCont::pointer>::value, _TCont>::type>& that) : current(that.base()) {}

        iterator_type base() const { return this->current; }

        reference operator*() const
        {
            return *this->current;
        }

        pointer operator->() const
        {
            return this->current;
        }

        reference operator[](difference_type n) const
        {
            return this->current[n];
        }

        _pointer_iterator& operator++()
        {
            ++this->current;
            return *this;
        }

        _pointer_iterator& operator--()
        {
            --this->current;
            return *this;
        }

        _pointer_iterator operator++(int)
        {
            return _pointer_iterator(this->current++);
        }

        _pointer_iterator operator--(int)
        {
            return _pointer_iterator(this->current--);
        }

        _pointer_iterator operator+(difference_type n) const
        {
            return _pointer_iterator(this->current + n);
        }

        _pointer_iterator operator-(difference_type n) const
        {
            return _pointer_iterator(this->current - n);
        }

        _pointer_iterator& operator+=(difference_type n)
        {
            this->current += n;
            return *this;
        }

        _pointer_iterator& operator-=(difference_type n)
        {
            this->current -= n;
            return *this;
        }
    };

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline bool operator==(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline bool operator!=(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline bool operator<(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return lhs.base() < rhs.base();
    }

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline bool operator<=(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline bool operator>(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return rhs < lhs;
    }

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline bool operator>=(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename TIter, typename _TCont>
    inline _pointer_iterator<TIter, _TCont> operator+(
        typename _pointer_iterator<TIter, _TCont>::difference_type n,
        const _pointer_iterator<TIter, _TCont>& it)
    {
        return _pointer_iterator<TIter, _TCont>(it.base() + n);
    }

    template <typename TIter, typename _TCont>
    inline typename _pointer_iterator<TIter, _TCont>::difference_type operator-(
        const _pointer_iterator<TIter, _TCont>& lhs,
        const _pointer_iterator<TIter, _TCont>& rhs)
    {
        return lhs.base() - rhs.base();
    }

    template <typename TIterFirst, typename TIterSecond, typename _TCont>
    inline typename _pointer_iterator<TIterFirst, _TCont>::difference_type operator-(
        const _pointer_iterator<TIterFirst, _TCont>& lhs,
        const _pointer_iterator<TIterSecond, _TCont>& rhs)
    {
        return lhs.base() - rhs.base();
    }
}
