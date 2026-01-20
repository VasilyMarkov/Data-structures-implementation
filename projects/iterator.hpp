/*
 * Copyright (c) 2026 VasilyMarkov
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#pragma once 

#include <cstddef>
#include <memory>
#include <type_traits>
#include <iostream>
#include <iterator>

template<
	typename Derived,
	typename Value,
	typename Category,
	typename Reference = Value&,
	typename DiffT = std::ptrdiff_t
>
class iterator_facade {
public:
    using value_type = std::remove_const_t<Value>;
    using reference = Reference;
    using pointer = value_type*;
    using iterator_category = Category;
    using difference_type = DiffT;

    reference operator*() const { return asDerived().dereference(); }
    pointer operator->() { return std::addressof(asDerived().dereference()); }

    Derived& operator++() { asDerived().increment(); return asDerived(); }

    Derived operator++(int) {
        auto tmp = asDerived();
        asDerived().increment();
        return tmp;
    }

    Derived& operator--() { asDerived().decrement(); return asDerived(); }

    Derived operator--(int) {
        auto tmp = asDerived();
        asDerived().decrement();
        return tmp;
    }

    Derived& operator+=(difference_type n) { std::advance(asDerived(), n); return asDerived(); }
    Derived& operator-=(difference_type n) { std::advance(asDerived(), -n); return asDerived(); }

    friend bool operator==(const iterator_facade& lhs, const iterator_facade& rhs) {
        return lhs.asDerived().equal(rhs.asDerived());
    }

    friend bool operator!=(const iterator_facade& lhs, const iterator_facade& rhs)
    {
        return !(lhs == rhs);
    }
    Derived& asDerived() { return *static_cast<Derived*>(this); }
    const Derived& asDerived() const { return *static_cast<const Derived*>(this); }
};
