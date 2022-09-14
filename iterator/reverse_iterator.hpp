/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator.hpp"
#include "iterator_traits.hpp"

#include <memory.hpp>

namespace ft
{
    template <class TIter>
    class reverse_iterator
        : public iterator<typename iterator_traits<TIter>::iterator_category,
                          typename iterator_traits<TIter>::value_type,
                          typename iterator_traits<TIter>::difference_type,
                          typename iterator_traits<TIter>::pointer,
                          typename iterator_traits<TIter>::reference>
    {
    protected:
        typedef iterator_traits<TIter> _Traits;

    public:
        typedef TIter iterator_type;
        typedef typename _Traits::difference_type iterator_category;
        typedef typename _Traits::difference_type value_type;
        typedef typename _Traits::difference_type difference_type;
        typedef typename _Traits::pointer pointer;
        typedef typename _Traits::reference reference;

    protected:
        TIter current;

        reverse_iterator() : current() {}
        explicit reverse_iterator(iterator_type current) : current(current) {}
        template <class U>
        reverse_iterator(const reverse_iterator<U>& that) : current(that.base()) {}

        iterator_type base() const { return current; }

        reference operator*() const
        {
            TIter tmp = current;
            return *--tmp;
        }

        pointer operator->() const
        {
            return ft::addressof(*(*this));
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }

        reverse_iterator& operator++()
        {
            --current;
            return *this;
        }

        reverse_iterator& operator--()
        {
            ++current;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator tmp = *this;
            --current;
            return tmp;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator tmp = *this;
            ++current;
            return tmp;
        }

        reverse_iterator operator+(difference_type n) const
        {
            return reverse_iterator(current - n);
        }

        reverse_iterator operator-(difference_type n) const
        {
            return reverse_iterator(current + n);
        }

        reverse_iterator& operator+=(difference_type n) const
        {
            current -= n;
            return *this;
        }

        reverse_iterator& operator-=(difference_type n) const
        {
            current += n;
            return *this;
        }
    };

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator==(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator!=(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator<(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator<=(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator>(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return rhs < lhs;
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator>=(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename TIter>
    inline reverse_iterator<TIter> operator+(
        typename reverse_iterator<TIter>::difference_type n,
        const reverse_iterator<TIter>& it)
    {
        return reverse_iterator<TIter>(it.base() - n);
    }

    template <typename TIter>
    inline typename reverse_iterator<TIter>::difference_type operator-(
        const reverse_iterator<TIter>& lhs,
        const reverse_iterator<TIter>& rhs)
    {
        return rhs.base() - lhs.base();
    }
}