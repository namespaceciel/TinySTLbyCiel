#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_LVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_LVALUE_REFERENCE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_lvalue_reference : false_type {};

	template<class T>
	struct is_lvalue_reference<T&> : true_type {};

	template<class T>
	inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_LVALUE_REFERENCE_H_