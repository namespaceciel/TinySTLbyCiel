#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_EMPTY_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_EMPTY_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_empty : bool_constant<__is_empty(T)> {};

	template<class T>
	inline constexpr bool is_empty_v = is_empty<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_EMPTY_H_