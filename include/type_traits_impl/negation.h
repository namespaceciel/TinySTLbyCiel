#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_NEGATION_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_NEGATION_H_

#include "integral_constant.h"

namespace ciel {

	template<class B>
	struct negation : bool_constant<!bool(B::value)> {};

	template<class B>
	inline constexpr bool negation_v = negation<B>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_NEGATION_H_