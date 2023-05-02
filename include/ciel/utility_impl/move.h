#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_MOVE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_MOVE_H_

#include "../type_traits_impl/remove_reference.h"

namespace ciel {

	template<class T>
	constexpr remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<remove_reference_t<T>&&>(t);
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_MOVE_H_