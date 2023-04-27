#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_VOLATILE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_VOLATILE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_volatile : false_type {};

	template<class T>
	struct is_volatile<volatile T> : true_type {};

	template<class T>
	inline constexpr bool is_volatile_v = is_volatile<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_VOLATILE_H_