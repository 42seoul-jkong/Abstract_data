/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <iterator.hpp>
#include <vector.hpp>

#include <cstddef>
#include <memory>

namespace ft
{
    template <typename T, typename TContainer = ft::vector<T> >
    class stack
    {
    public:
        typedef TContainer container_type;
        typedef typename TContainer::value_type value_type;
        typedef typename TContainer::size_type size_type;
        typedef typename TContainer::reference reference;
        typedef typename TContainer::const_reference const_reference;

    protected:
        container_type c;

    public:
        explicit stack(const container_type& c = container_type());
        stack(const stack& that);
        ~stack();
        stack& operator=(const stack& that);

        reference top();
        const_reference top() const;

        bool empty() const;
        size_type size() const;

        void push(const value_type& value);
        void pop();
    };

    template <typename T, typename TContainer>
    inline bool operator==(
        const stack<T, TContainer>& lhs,
        const stack<T, TContainer>& rhs);

    template <typename T, typename TContainer>
    inline bool operator!=(
        const stack<T, TContainer>& lhs,
        const stack<T, TContainer>& rhs);

    template <typename T, typename TContainer>
    inline bool operator<(
        const stack<T, TContainer>& lhs,
        const stack<T, TContainer>& rhs);

    template <typename T, typename TContainer>
    inline bool operator<=(
        const stack<T, TContainer>& lhs,
        const stack<T, TContainer>& rhs);

    template <typename T, typename TContainer>
    inline bool operator>(
        const stack<T, TContainer>& lhs,
        const stack<T, TContainer>& rhs);

    template <typename T, typename TContainer>
    inline bool operator>=(
        const stack<T, TContainer>& lhs,
        const stack<T, TContainer>& rhs);
}
