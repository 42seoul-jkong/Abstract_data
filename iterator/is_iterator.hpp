/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator_traits.hpp"

namespace ft
{
    template <typename T, typename = void>
    struct is_iterator : ft::false_type
    {
    };

    template <typename T>
    struct is_iterator<T, typename ft::void_t<typename ft::iterator_traits<T>::iterator_category>::type> : ft::true_type
    {
    };
}
