/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <bool B, class T = void>
    struct enable_if
    {
        typedef T type;
    };

    template <class T>
    struct enable_if<false, T>
    {
    };
}
