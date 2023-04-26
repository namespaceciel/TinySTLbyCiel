#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_HAS_VIRTUAL_DESTRUCTOR_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_HAS_VIRTUAL_DESTRUCTOR_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct has_virtual_destructor : bool_constant<__has_virtual_destructor(T)> {};

	template<class T>
	inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_HAS_VIRTUAL_DESTRUCTOR_H_