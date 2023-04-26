#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

	template<class T>
	inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_TRIVIALLY_COPYABLE_H_