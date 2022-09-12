/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"

namespace ft
{
    // Default
    template <class T>
    struct is_reference : public ft::false_type
    {
    };

    // Reference
    template <class T>
    struct is_reference<T&> : public ft::true_type
    {
    };
}
