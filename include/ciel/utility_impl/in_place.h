#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_IN_PLACE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_IN_PLACE_H_

#include <cstddef>
#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/remove_cvref.h>

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

    template<class T, class U = ciel::remove_cvref_t<T>>
    struct is_inplace_type : ciel::false_type {};

    template<class T, class In>
    struct is_inplace_type<T, in_place_type_t<In>> : ciel::true_type {};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_IN_PLACE_H_