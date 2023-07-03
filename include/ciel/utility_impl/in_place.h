#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_IN_PLACE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_IN_PLACE_H_

#include <cstddef>

namespace ciel {

    struct in_place_t { explicit in_place_t() = default; };

    inline constexpr in_place_t in_place{};

    template<class T>
    struct in_place_type_t { explicit in_place_type_t() = default; };

    template<class T>
    inline constexpr in_place_type_t<T> in_place_type{};

    template<size_t I>
    struct in_place_index_t { explicit in_place_index_t() = default; };

    template<size_t I>
    inline constexpr in_place_index_t<I> in_place_index{};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_IN_PLACE_H_