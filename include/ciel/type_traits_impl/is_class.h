#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CLASS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CLASS_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_class : bool_constant<__is_class(T)> {};

	template<class T>
	inline constexpr bool is_class_v = is_class<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CLASS_H_