/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    /// Arithmetic operations
    template <typename T>
    struct plus
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs + rhs; }
    };
    template <typename T>
    struct minus
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs - rhs; }
    };
    template <typename T>
    struct multiplies
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs * rhs; }
    };
    template <typename T>
    struct divides
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs / rhs; }
    };
    template <typename T>
    struct modules
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs % rhs; }
    };
    template <typename T>
    struct negate
    {
        T operator()(const T& arg) const { return -arg; }
    };

    /// Comparisons
    template <typename T>
    struct equal_to
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs == rhs; }
    };
    template <typename T>
    struct not_equal_to
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs != rhs; }
    };
    template <typename T>
    struct greater
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs > rhs; }
    };
    template <typename T>
    struct less
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
    };
    template <typename T>
    struct greater_equal
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs >= rhs; }
    };
    template <typename T>
    struct less_equal
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs <= rhs; }
    };

    /// Logical operations
    template <typename T>
    struct logical_and
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs && rhs; }
    };
    template <typename T>
    struct logical_or
    {
        bool operator()(const T& lhs, const T& rhs) const { return lhs || rhs; }
    };
    template <typename T>
    struct logical_not
    {
        bool operator()(const T& arg) const { return !arg; }
    };

    /// Bitwise operations
    template <typename T>
    struct bit_and
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs & rhs; }
    };
    template <typename T>
    struct bit_or
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs | rhs; }
    };
    template <typename T>
    struct bit_xor
    {
        T operator()(const T& lhs, const T& rhs) const { return lhs ^ rhs; }
    };
    template <typename T>
    struct bit_not
    {
        T operator()(const T& arg) const { return ~arg; }
    };
}
