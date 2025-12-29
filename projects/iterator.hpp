#pragma once 

#include <cstddef>
#include <memory>
#include <type_traits>
#include <iostream>

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

    reference operator*() { return asDerived().dereference(); }
    pointer operator->() { return std::addressof(asDerived().dereference()); }

    Derived& operator++() { asDerived().increment(); return asDerived(); }
    Derived& operator--() { asDerived().decrement(); return asDerived(); }
    Derived& operator+=(size_t n) {
        advance(n);
        return asDerived();
    }

    constexpr void advance(size_t n) {
        auto dist = difference_type(n);
        if constexpr(std:is_base_v<std::bidirectional_iterator_tag, iterator_category>) {
            while(dist > 0) {
                ++(*this);
                --dist;
            }
        }
        else if (std:is_base_v<std::forward_iterator_tag, iterator_category>) {

        }
    }

    friend bool operator==(const iterator_facade& lhs, const iterator_facade& rhs) {
        return lhs.asDerived().equal(rhs.asDerived());
    }
    
    friend bool operator!=(const iterator_facade& lhs, const iterator_facade& rhs) {
        return !(lhs == rhs);
    }

    Derived& asDerived() { return *static_cast<Derived*>(this); }
    const Derived& asDerived() const { return *static_cast<const Derived*>(this); }
};
