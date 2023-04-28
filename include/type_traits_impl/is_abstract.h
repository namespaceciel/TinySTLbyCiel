#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_ABSTRACT_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_ABSTRACT_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_abstract : bool_constant<__is_abstract(T)> {};

	template<class T>
	inline constexpr bool is_abstract_v = is_abstract<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_ABSTRACT_H_