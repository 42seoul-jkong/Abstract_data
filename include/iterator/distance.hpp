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
        typename ft::iterator_traits<TIter>::difference_type distance(TIter first, TIter last, ft::input_iterator_tag)
        {
            typename ft::iterator_traits<TIter>::difference_type result = typename ft::iterator_traits<TIter>::difference_type();
            for (TIter val = first; val != last; ++val)
            {
                ++result;
            }
            return result;
        }

        template <typename TIter>
        typename ft::iterator_traits<TIter>::difference_type distance(TIter first, TIter last, ft::random_access_iterator_tag)
        {
            return last - first;
        }
    }

    template <typename TIter>
    typename ft::iterator_traits<TIter>::difference_type distance(TIter first, TIter last)
    {
        return _internal::distance(first, last, typename ft::iterator_traits<TIter>::iterator_category());
    }
}
