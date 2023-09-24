/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator_traits.hpp"

#include <type_traits.hpp>

#include <iterator>

namespace ft
{
    template <typename T, typename = void>
    struct is_iterator : ft::false_type
    {
    };

    template <typename T>
    struct is_iterator<T, typename ft::make_void<typename ft::iterator_traits<T>::iterator_category>::type> : ft::true_type
    {
    };

    namespace _internal
    {
        template <typename TTag, typename T, typename = void>
        struct is_specific_iterator : ft::false_type
        {
        };

        template <typename TTag, typename T>
        struct is_specific_iterator<TTag, T, typename ft::enable_if<ft::is_convertible<typename ft::iterator_traits<T>::iterator_category, TTag>::value, void>::type> : ft::true_type
        {
        };
    }

    template <typename T>
    struct is_input_iterator : _internal::is_specific_iterator<std::input_iterator_tag, T>
    {
    };

    template <typename T>
    struct is_output_iterator : _internal::is_specific_iterator<std::output_iterator_tag, T>
    {
    };

    template <typename T>
    struct is_forward_iterator : _internal::is_specific_iterator<std::forward_iterator_tag, T>
    {
    };

    template <typename T>
    struct is_bidirectional_iterator : _internal::is_specific_iterator<std::bidirectional_iterator_tag, T>
    {
    };

    template <typename T>
    struct is_random_access_iterator : _internal::is_specific_iterator<std::random_access_iterator_tag, T>
    {
    };
}
