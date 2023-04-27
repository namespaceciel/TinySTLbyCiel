#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_SIGNED_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_SIGNED_H_

#include "is_arithmetic.h"

namespace ciel {

	template<class T, bool = is_arithmetic<T>::value>
	struct is_signed_helper : bool_constant<T(-1) < T(0)> {};

	template<class T>
	struct is_signed_helper<T, false> : false_type {};

	template<class T>
	struct is_signed : is_signed_helper<T>::type {};

	template<class T>
	inline constexpr bool is_signed_v = is_signed<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_SIGNED_H_