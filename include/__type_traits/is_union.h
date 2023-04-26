#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNION_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNION_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_union : bool_constant<__is_union(T)> {};

	template<class T>
	inline constexpr bool is_union_v = is_union<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNION_H_