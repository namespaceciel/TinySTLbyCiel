#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_RVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_RVALUE_REFERENCE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_rvalue_reference : false_type {};

	template<class T>
	struct is_rvalue_reference<T&&> : true_type {};

	template<class T>
	inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_RVALUE_REFERENCE_H_