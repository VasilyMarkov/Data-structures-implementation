/*
 * Copyright (c) 2026 VasilyMarkov
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#pragma once 

#include <type_traits>
#include <iterator>
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

    friend bool operator!=(const iterator_facade& lhs, const iterator_facade& rhs) {
        return !(lhs == rhs);
    }
    Derived& asDerived() { return *static_cast<Derived*>(this); }
    const Derived& asDerived() const { return *static_cast<const Derived*>(this); }
};

template<typename T>
void print_ref_category(T&&) {
    if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
        std::cout << "const ";
    }
    if constexpr(std::is_lvalue_reference_v<T>) {
        std::cout << "&";
    } else {
        std::cout << "&&";
    }
    std::cout << std::endl;
}

template<typename... Its>
using common_it_tag = std::common_type_t<typename std::iterator_traits<Its>::iterator_category...>;

template<typename... Its>
class zip_iterator: public iterator_facade<
    zip_iterator<Its...>,
    std::tuple<typename std::iterator_traits<std::remove_cvref_t<Its>>::value_type...>,
    common_it_tag<std::remove_cvref_t<Its>...>,
    std::tuple<typename std::iterator_traits<std::remove_cvref_t<Its>>::reference...>>
{

    template<class Reference> //https://quuxplusone.github.io/blog/2019/02/06/arrow-proxy/
    struct arrow_proxy {
        Reference r;
        Reference *operator->() {
            return &r;
        }
    };

    using BaseType = iterator_facade<
        zip_iterator<Its...>, 
        std::tuple<typename std::iterator_traits<std::remove_cvref_t<Its>>::value_type...>, 
        common_it_tag<std::remove_cvref_t<Its>...>,
        std::tuple<typename std::iterator_traits<std::remove_cvref_t<Its>>::reference...>
    >;
public:
    using value_type = typename BaseType::value_type;
    using reference = typename BaseType::reference;
    using iterator_category = common_it_tag<std::remove_cvref_t<Its>...>;
    using difference_type = typename BaseType::difference_type;
    using pointer = arrow_proxy<reference>;

    constexpr zip_iterator(Its... args): iterators_(args...) {}
    
    reference dereference() const noexcept { return std::apply([](auto&... its){ return reference(*its...); }, iterators_); }
    
    void increment() { std::apply([](auto&... its){ (++its, ...); }, iterators_); };

    bool equal(const zip_iterator& zit) const noexcept { return std::get<0>(zit.iterators_) == std::get<0>(iterators_); };

private:
    std::tuple<Its...> iterators_;
};

template<typename... Ranges>
class zip_range {
    
};
