#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_AGGREGATE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_AGGREGATE_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	template<class T>
	struct is_aggregate : bool_constant<__is_aggregate(T)> {};

	template<class T>
	inline constexpr bool is_aggregate_v = is_aggregate<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_AGGREGATE_H_