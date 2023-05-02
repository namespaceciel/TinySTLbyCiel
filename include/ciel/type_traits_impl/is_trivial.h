#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TRIVIAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TRIVIAL_H_

#include <ciel/type_traits_impl/is_trivially_copyable.h>
#include <ciel/type_traits_impl/is_default_constructible.h>

namespace ciel {

	template<class T>
	struct is_trivial : bool_constant<is_trivially_copyable_v<T> && is_trivially_default_constructible_v<T>> {};

	template<class T>
	inline constexpr bool is_trivial_v = is_trivial<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TRIVIAL_H_