#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_UNBOUNDED_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_UNBOUNDED_ARRAY_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_unbounded_array : ciel::false_type {};

	template<class T>
	struct is_unbounded_array<T[]> : ciel::true_type {};

	template<class T>
	inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_UNBOUNDED_ARRAY_H_