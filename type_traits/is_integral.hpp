/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"

namespace ft
{
    // Default
    template <typename T>
    struct is_integral : public ft::false_type
    {
    };

    // Remove cv-qualified variants
    template <typename T>
    struct is_integral<const T> : public is_integral<T>
    {
    };
    template <typename T>
    struct is_integral<volatile const T> : public is_integral<T>
    {
    };
    template <typename T>
    struct is_integral<volatile T> : public is_integral<T>
    {
    };

    // Unsigned
    template <>
    struct is_integral<unsigned char> : public ft::true_type
    {
    };
    template <>
    struct is_integral<unsigned short> : public ft::true_type
    {
    };
    template <>
    struct is_integral<unsigned int> : public ft::true_type
    {
    };
    template <>
    struct is_integral<unsigned long> : public ft::true_type
    {
    };

    // Signed
    template <>
    struct is_integral<signed char> : public ft::true_type
    {
    };
    template <>
    struct is_integral<short> : public ft::true_type
    {
    };
    template <>
    struct is_integral<int> : public ft::true_type
    {
    };
    template <>
    struct is_integral<long> : public ft::true_type
    {
    };

    // Intrinsic
    template <>
    struct is_integral<char> : public ft::true_type
    {
    };
    template <>
    struct is_integral<bool> : public ft::true_type
    {
    };
}
