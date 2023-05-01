#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_DISJUNCTION_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_DISJUNCTION_H_

#include "integral_constant.h"
#include "conditional.h"

namespace ciel {

	template<class...>
	struct disjunction : false_type {};

	template<class B1>
	struct disjunction<B1> : B1 {};

	template<class B1, class... Bn>
	struct disjunction<B1, Bn...> : conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};

	template<class... B>
	inline constexpr bool disjunction_v = disjunction<B...>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_DISJUNCTION_H_