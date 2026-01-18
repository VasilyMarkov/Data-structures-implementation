#pragma once
#include <type_traits>

template<typename T>
concept DefaultConstructible = requires {
    requires std::is_default_constructible_v<std::remove_cvref_t<T>>;
};