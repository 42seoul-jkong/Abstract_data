/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"

namespace ft
{
    // Default
    template <typename T>
    struct is_const : ft::false_type
    {
    };

    // Reference
    template <typename T>
    struct is_const<const T> : ft::true_type
    {
    };
}
