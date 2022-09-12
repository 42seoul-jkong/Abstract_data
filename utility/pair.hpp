/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <typename TFirst, typename TSecond>
    struct pair
    {
        typedef TFirst first_type;
        typedef TSecond second_type;

        TFirst first;
        TSecond second;

        pair() : first(), second() {}
        pair(const TFirst& first, const TSecond& second) : first(first), second(second) {}
        template <typename UFirst, typename USecond>
        pair(const pair<UFirst, USecond>& pair) : first(pair.first), second(pair.second) {}

        pair& operator=(const pair& that)
        {
            this->first = that.first;
            this->second = that.second;
        }
    };

    template <class TFirst, class TSecond>
    inline pair<TFirst, TSecond> make_pair(
        TFirst first, TSecond second)
    {
        return pair<TFirst, TSecond>(first, second);
    }

    template <typename TFirst, typename TSecond>
    inline bool operator==(
        const pair<TFirst, TSecond>& lhs,
        const pair<TFirst, TSecond>& rhs)
    {
        return lhs.first == rhs.first &&
               lhs.second == rhs.second;
    }

    template <typename TFirst, typename TSecond>
    inline bool operator!=(
        const pair<TFirst, TSecond>& lhs,
        const pair<TFirst, TSecond>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TFirst, typename TSecond>
    inline bool operator<(
        const pair<TFirst, TSecond>& lhs,
        const pair<TFirst, TSecond>& rhs)
    {
        if (lhs.first < rhs.first)
        {
            return true;
        }
        else if (rhs.first < lhs.first)
        {
            return false;
        }
        // lhs.first == rhs.first
        else if (lhs.second < rhs.second)
        {
            return true;
        }
        return false;
    }

    template <typename TFirst, typename TSecond>
    inline bool operator<=(
        const pair<TFirst, TSecond>& lhs,
        const pair<TFirst, TSecond>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename TFirst, typename TSecond>
    inline bool operator>(
        const pair<TFirst, TSecond>& lhs,
        const pair<TFirst, TSecond>& rhs)
    {
        return rhs < lhs;
    }

    template <typename TFirst, typename TSecond>
    inline bool operator>=(
        const pair<TFirst, TSecond>& lhs,
        const pair<TFirst, TSecond>& rhs)
    {
        return !(lhs < rhs);
    }
}
