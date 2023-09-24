/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator_tags.hpp"
#include "iterator_traits.hpp"

namespace ft
{
    namespace _internal
    {
        template <typename TIter>
        void advance(TIter& it, typename ft::iterator_traits<TIter>::difference_type n, ft::input_iterator_tag)
        {
            while (n > 0)
            {
                --n;
                ++it;
            }
        }

        template <typename TIter>
        void advance(TIter& it, typename ft::iterator_traits<TIter>::difference_type n, ft::bidirectional_iterator_tag)
        {
            while (n > 0)
            {
                --n;
                ++it;
            }
            while (n < 0)
            {
                ++n;
                --it;
            }
        }

        template <typename TIter>
        void advance(TIter& it, typename ft::iterator_traits<TIter>::difference_type n, ft::random_access_iterator_tag)
        {
            it += n;
        }
    }

    template <typename TIter, typename TDistance>
    inline void advance(TIter& it, TDistance n)
    {
        _internal::advance(it, typename ft::iterator_traits<TIter>::difference_type(n), typename ft::iterator_traits<TIter>::iterator_category());
    }
}
