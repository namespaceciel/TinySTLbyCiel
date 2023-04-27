#ifndef TINYSTLBYCIEL_INCLUDE___UTILITY_MOVE_H_
#define TINYSTLBYCIEL_INCLUDE___UTILITY_MOVE_H_

#include "../type_traits.h"

namespace ciel {

	template<class T>
	constexpr remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<remove_reference_t<T>&&>(t);
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___UTILITY_MOVE_H_