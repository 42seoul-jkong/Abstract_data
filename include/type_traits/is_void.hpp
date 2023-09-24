/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"

namespace ft
{
    // Default
    template <typename T>
    struct is_void : ft::false_type
    {
    };

    // Void
    template <>
    struct is_void<void> : ft::true_type
    {
    };
    template <>
    struct is_void<const void> : ft::true_type
    {
    };
    template <>
    struct is_void<const volatile void> : ft::true_type
    {
    };
    template <>
    struct is_void<volatile void> : ft::true_type
    {
    };
}
