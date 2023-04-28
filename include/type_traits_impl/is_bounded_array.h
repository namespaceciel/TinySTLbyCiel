#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_BOUNDED_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_BOUNDED_ARRAY_H_

#include "integral_constant.h"
#include <cstddef>

namespace ciel {

	template<class T>
	struct is_bounded_array : false_type {};

	template<class T, size_t N>
	struct is_bounded_array<T[N]> : true_type {};

	template<class T>
	inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_BOUNDED_ARRAY_H_