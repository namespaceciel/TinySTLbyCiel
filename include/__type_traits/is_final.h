#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_FINAL_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_FINAL_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_final : bool_constant<__is_final(T)> {};

	template<class T>
	inline constexpr bool is_final_v = is_final<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_FINAL_H_