/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void,
              typename T6 = void, typename T7 = void, typename T8 = void, typename T9 = void>
    struct make_void
    {
        typedef void type;
    };
}
