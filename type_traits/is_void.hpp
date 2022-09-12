/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"

namespace ft
{
    // Default
    template <class T>
    struct is_void : public ft::false_type
    {
    };

    // Void
    template <>
    struct is_void<void> : public ft::true_type
    {
    };
    template <>
    struct is_void<const void> : public ft::true_type
    {
    };
    template <>
    struct is_void<const volatile void> : public ft::true_type
    {
    };
    template <>
    struct is_void<volatile void> : public ft::true_type
    {
    };
}
