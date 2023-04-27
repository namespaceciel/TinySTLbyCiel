#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNBOUNDED_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNBOUNDED_ARRAY_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_unbounded_array : false_type {};

	template<class T>
	struct is_unbounded_array<T[]> : true_type {};

	template<class T>
	inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNBOUNDED_ARRAY_H_