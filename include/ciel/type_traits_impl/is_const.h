#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONST_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONST_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel{

	template<class T>
	struct is_const : ciel::false_type {};

	template<class T>
	struct is_const<const T> : ciel::true_type {};

	template<class T>
	inline constexpr bool is_const_v = is_const<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONST_H_