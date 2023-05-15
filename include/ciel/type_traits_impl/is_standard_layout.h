#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_STANDARD_LAYOUT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_STANDARD_LAYOUT_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_standard_layout : ciel::bool_constant<__is_standard_layout(T)> {};

	template<class T>
	inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_STANDARD_LAYOUT_H_