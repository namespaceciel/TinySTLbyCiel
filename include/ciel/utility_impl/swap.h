#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_SWAP_H
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_SWAP_H

#include <stddef.h>
#include <ciel/type_traits_impl/is_swappable.h>
#include <ciel/utility_impl/move.h>

namespace ciel {

    template<class T>
    constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>);

    template<class T, size_t N>
    constexpr void swap(T (& a)[N], T (& b)[N]) noexcept(is_nothrow_swappable_v<T>) {
        for (size_t i = 0; i < N; ++i) {
            swap(a[i], b[i]);
        }
    }

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_SWAP_H