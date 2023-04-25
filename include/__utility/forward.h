#ifndef TINYSTLBYCIEL_INCLUDE___UTILITY_FORWARD_H_
#define TINYSTLBYCIEL_INCLUDE___UTILITY_FORWARD_H_

#include "../type_traits.h"

namespace ciel {

	template<class T>
	constexpr T&& forward(remove_reference_t<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	template<class T>
	constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
		return static_cast<T&&>(t);
	}
}

#endif //TINYSTLBYCIEL_INCLUDE___UTILITY_FORWARD_H_