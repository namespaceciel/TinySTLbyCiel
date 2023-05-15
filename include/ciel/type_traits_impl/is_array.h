#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ARRAY_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <cstddef>

namespace ciel{

	template<class T>
	struct is_array : ciel::false_type {};

	template<class T>
	struct is_array<T[]> : ciel::true_type {};

	template<class T, size_t N>
	struct is_array<T[N]> : ciel::true_type {};

	template<class T>
	inline constexpr bool is_array_v = is_array<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ARRAY_H_