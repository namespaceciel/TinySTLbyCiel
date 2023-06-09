#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_EXTENT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_EXTENT_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <cstddef>

namespace ciel {

	template<class T, unsigned N = 0>
	struct extent : ciel::integral_constant<size_t, 0> {};

	template<class T>
	struct extent<T[], 0> : ciel::integral_constant<size_t, 0> {};

	template<class T, unsigned N>
	struct extent<T[], N> : extent<T, N - 1> {};

	template<class T, size_t I>
	struct extent<T[I], 0> : ciel::integral_constant<size_t, I> {};

	template<class T, size_t I, unsigned N>
	struct extent<T[I], N> : extent<T, N - 1> {};

	template<class T, unsigned N = 0>
	inline constexpr size_t extent_v = extent<T, N>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_EXTENT_H_