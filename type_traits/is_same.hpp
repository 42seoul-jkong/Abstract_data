/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"

namespace ft
{
    // Default
    template <class T, class U>
    struct is_same : public false_type
    {
    };

    // Same
    template <class T>
    struct is_same<T, T> : public true_type
    {
    };
}
