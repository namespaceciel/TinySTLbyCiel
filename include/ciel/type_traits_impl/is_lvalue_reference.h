#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_LVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_LVALUE_REFERENCE_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_lvalue_reference : ciel::false_type {};

	template<class T>
	struct is_lvalue_reference<T&> : ciel::true_type {};

	template<class T>
	inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_LVALUE_REFERENCE_H_