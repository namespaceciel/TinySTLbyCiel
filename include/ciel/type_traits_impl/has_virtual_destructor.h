#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_HAS_VIRTUAL_DESTRUCTOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_HAS_VIRTUAL_DESTRUCTOR_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct has_virtual_destructor : bool_constant<__has_virtual_destructor(T)> {};

	template<class T>
	inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_HAS_VIRTUAL_DESTRUCTOR_H_