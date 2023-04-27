#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_AGGREGATE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_AGGREGATE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_aggregate : bool_constant<__is_aggregate(T)> {};

	template<class T>
	inline constexpr bool is_aggregate_v = is_aggregate<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_AGGREGATE_H_