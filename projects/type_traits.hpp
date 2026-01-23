#pragma once

#include <type_traits>
#include <iterator>

/*
 * Copyright (c) 2026 VasilyMarkov
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

template<typename... Its>
using common_it_tag = std::common_type_t<typename std::iterator_traits<Its>::iterator_category...>;

template<typename T, typename = void>
struct value_type_of {
    using type = void;
};

template<typename T>
struct value_type_of<T, std::void_t<typename T::value_type>> {
    using type = typename T::value_type;
};

template<typename T>
struct value_type_of<T*> {
    using type = T;
};

template<typename T>
struct value_type_of<T(&)[]> {
    using type = T;
};

template<typename T>
using value_type_of_t = value_type_of<T>;

template<typename T, typename = void>
struct iterator_of {
    using type = void;
};

template<typename T>
struct iterator_of<T, std::void_t<typename T::iterator>> {
    using type = typename T::iterator;
};

template<typename T>
struct iterator_of<T*> {
    using type = T*;
};

template<typename T, std::size_t N>
struct iterator_of<T[N]> {
    using type = T*;
};

template<typename T>
using iterator_of_t = iterator_of<T>;