#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TRIVIALLY_COPYABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TRIVIALLY_COPYABLE_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

	template<class T>
	inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TRIVIALLY_COPYABLE_H_