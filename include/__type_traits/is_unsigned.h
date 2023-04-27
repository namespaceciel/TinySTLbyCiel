#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNSIGNED_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNSIGNED_H_

#include "is_arithmetic.h"

namespace ciel {

	template<class T, bool = is_arithmetic<T>::value>
	struct is_unsigned_helper : bool_constant<T(0) < T(-1)> {};

	template<class T>
	struct is_unsigned_helper<T, false> : false_type {};

	template<class T>
	struct is_unsigned : is_unsigned_helper<T>::type {};

	template<class T>
	inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_UNSIGNED_H_