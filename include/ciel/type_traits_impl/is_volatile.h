#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOLATILE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOLATILE_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_volatile : ciel::false_type {};

	template<class T>
	struct is_volatile<volatile T> : ciel::true_type {};

	template<class T>
	inline constexpr bool is_volatile_v = is_volatile<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOLATILE_H_