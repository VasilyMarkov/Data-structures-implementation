/*
 * Copyright (c) 2026 VasilyMarkov
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#pragma once 

#include <type_traits>
#include <iterator>
#include <iostream>
#include "type_traits.hpp"

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
class zip_iterator: public iterator_facade<
    zip_iterator<Its...>,
    std::tuple<typename std::iterator_traits<std::remove_cvref_t<Its>>::value_type...>,
    common_it_tag<std::remove_cvref_t<Its>...>,
    std::tuple<typename std::iterator_traits<std::remove_cvref_t<Its>>::reference...>>
{
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
    using pointer = void;

    constexpr zip_iterator(Its... args): iterators_(args...) {}
    
    reference dereference() const noexcept { 
        return std::apply([](auto&... its){ return reference(*its...); }, iterators_); 
    }
    
    void increment() { std::apply([](auto&... its){ (++its, ...); }, iterators_); };
    void decrement() { std::apply([](auto&... its){ (--its, ...); }, iterators_); };

    bool equal(const zip_iterator& other) const noexcept { 
        auto impl = [this, &other]<std::size_t... I>(std::index_sequence<I...>){
            return ((std::get<I>(other.iterators_) == std::get<I>(iterators_)) && ...);
        };
        return impl(std::make_index_sequence<sizeof...(Its)>{}); 
    };

private:
    std::tuple<Its...> iterators_;
};

template<typename... Ranges>
class zip_range {
public:
    using value_type = std::tuple<value_type_of_t<Ranges>...>;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator = zip_iterator<decltype(std::begin(std::declval<Ranges&>()))...>;
    using difference_type = std::ptrdiff_t;

    template<typename... Args>
    zip_range(Args&&... args): ranges_(std::forward<Args>(args)...) {
        if(!is_equal_sizes()) {
            throw std::runtime_error("Trying to create zip_range with ranges based on different sizes");
        }
     }

    iterator begin() { return std::apply([](auto&&... ranges){ return iterator(std::begin(ranges)...); }, ranges_); }
    iterator end() { return std::apply([](auto&&... ranges){ return iterator(std::end(ranges)...); }, ranges_); }
private:

    constexpr bool is_equal_sizes() const {
        return [this]<std::size_t... I>(std::index_sequence<I...>){
            return ((std::size(std::get<I>(ranges_)) == std::size(std::get<I+1>(ranges_))) && ...);
        }(std::make_index_sequence<sizeof...(Ranges) - 1>{});
    }

    std::tuple<Ranges...> ranges_;
};

template<typename... Ranges>
auto make_zip_range(Ranges&&... ranges) {
    return zip_range<Ranges...>(std::forward<Ranges>(ranges)...); 
}