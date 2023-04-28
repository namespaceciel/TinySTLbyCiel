#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_EMPTY_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_EMPTY_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_empty : bool_constant<__is_empty(T)> {};

	template<class T>
	inline constexpr bool is_empty_v = is_empty<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_EMPTY_H_