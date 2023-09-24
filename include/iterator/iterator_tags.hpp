/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <iterator>

namespace ft
{
    typedef std::input_iterator_tag input_iterator_tag;
    typedef std::output_iterator_tag output_iterator_tag;
    typedef std::forward_iterator_tag forward_iterator_tag;
    typedef std::bidirectional_iterator_tag bidirectional_iterator_tag;
    typedef std::random_access_iterator_tag random_access_iterator_tag;
}
