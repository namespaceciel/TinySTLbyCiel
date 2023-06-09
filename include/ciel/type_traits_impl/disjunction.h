#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DISJUNCTION_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DISJUNCTION_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/conditional.h>

namespace ciel {

	template<class...>
	struct disjunction : ciel::false_type {};

	template<class B1>
	struct disjunction<B1> : B1 {};

	template<class B1, class... Bn>
	struct disjunction<B1, Bn...> : ciel::conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};

	template<class... B>
	inline constexpr bool disjunction_v = disjunction<B...>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DISJUNCTION_H_