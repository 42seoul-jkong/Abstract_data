/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <typename T, T Value>
    struct integral_constant
    {
        typedef T value_type;
        typedef integral_constant<T, Value> type;
        static const T value = Value;

        operator T() const { return Value; }
    };

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;
}
