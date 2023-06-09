#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_RANK_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_RANK_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <cstddef>

namespace ciel {

	template<class T>
	struct rank : ciel::integral_constant<size_t, 0> {};

	template<class T>
	struct rank<T[]> : ciel::integral_constant<size_t, rank<T>::value + 1> {};

	template<class T, size_t N>
	struct rank<T[N]> : ciel::integral_constant<size_t, rank<T>::value + 1> {};

	template<class T>
	inline constexpr size_t rank_v = rank<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_RANK_H_