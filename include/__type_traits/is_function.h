#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_FUNCTION_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_FUNCTION_H_

#include "is_const.h"
#include "is_reference.h"

namespace ciel {

	//A function or reference type is always cv-unqualified.
	template<class T>
	struct is_function : bool_constant<!is_const_v<const T> && !is_reference_v<T>> {};

	template<class T>
	inline constexpr bool is_function_v = is_function<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_FUNCTION_H_