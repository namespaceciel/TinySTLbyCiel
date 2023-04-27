#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_STANDARD_LAYOUT_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_STANDARD_LAYOUT_H_

#include "is_scalar.h"
#include "remove_all_extents.h"

namespace ciel {

	template<class T>
	struct is_standard_layout : bool_constant<__is_standard_layout(T)> {};

	template<class T>
	inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_STANDARD_LAYOUT_H_