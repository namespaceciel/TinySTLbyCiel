#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_FORWARD_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_FORWARD_H_

#include "../type_traits_impl/remove_reference.h"

namespace ciel {

	template<class T>
	constexpr T&& forward(remove_reference_t<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	template<class T>
	constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
		return static_cast<T&&>(t);
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_FORWARD_H_