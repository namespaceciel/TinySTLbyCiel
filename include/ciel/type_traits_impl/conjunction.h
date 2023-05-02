#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_CONJUNCTION_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_CONJUNCTION_H_

#include "integral_constant.h"
#include "conditional.h"

namespace ciel {

	//无成员时继承自 true_type
	template<class...>
	struct conjunction : true_type {};

	//否则，由 conditional 实现，继承自首个 false 成员（若全为 true 则选择最后一个）
	template<class B1>
	struct conjunction<B1> : B1 {};

	template<class B1, class... Bn>
	struct conjunction<B1, Bn...> : conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

	template<class... B>
	inline constexpr bool conjunction_v = conjunction<B...>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_CONJUNCTION_H_