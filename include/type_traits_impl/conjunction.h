#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_CONJUNCTION_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_CONJUNCTION_H_

#include "integral_constant.h"
#include "conditional.h"

namespace ciel {

	template<class...>
	struct conjunction : true_type {};

	template<class B1>
	struct conjunction<B1> : B1 {};

	template<class B1, class... Bn>
	struct conjunction<B1, Bn...> : conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_CONJUNCTION_H_