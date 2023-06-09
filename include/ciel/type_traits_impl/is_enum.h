#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ENUM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ENUM_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_enum : ciel::bool_constant<__is_enum(T)> {};

	template<class T>
	inline constexpr bool is_enum_v = is_enum<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ENUM_H_