#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNCTION_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNCTION_H_

#include <ciel/type_traits_impl/is_const.h>
#include <ciel/type_traits_impl/is_reference.h>

namespace ciel {

	//A function or reference type is always cv-unqualified.
	template<class T>
	struct is_function : ciel::bool_constant<!ciel::is_const_v<const T> && !ciel::is_reference_v<T>> {};

	template<class T>
	inline constexpr bool is_function_v = is_function<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNCTION_H_