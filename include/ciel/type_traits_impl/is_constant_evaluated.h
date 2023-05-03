#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONSTANT_EVALUATED_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONSTANT_EVALUATED_H_

namespace ciel {

	constexpr bool is_constant_evaluated() noexcept {
		return __builtin_is_constant_evaluated();
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONSTANT_EVALUATED_H_