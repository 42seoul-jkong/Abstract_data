/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>

namespace ft
{
    template <typename TSource, typename TDest>
    struct is_convertible
    {
    private:
        typedef char (&yes)[1];
        typedef char (&no)[2];

        static yes f(TDest*);
        static no f(...);

    public:
        static const bool value = sizeof((f)(static_cast<TSource*>(0))) == sizeof(yes);
    };

    template <typename TSource, typename TDest>
    struct is_convertible<TSource, TDest[]>
    {
        static const bool value = false;
    };

    template <typename TSource, typename TDest>
    struct is_convertible<TSource[], TDest[]> : is_convertible<TSource[1], TDest[1]>
    {
    };

    template <typename TSource, std::size_t N, typename TDest>
    struct is_convertible<TSource[N], TDest[]> : is_convertible<TSource[1], TDest[1]>
    {
    };
}
