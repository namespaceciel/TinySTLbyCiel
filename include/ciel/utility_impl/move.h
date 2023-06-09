#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_MOVE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_MOVE_H_

#include <ciel/type_traits_impl/remove_reference.h>

namespace ciel {

	template<class T>
	constexpr ciel::remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<ciel::remove_reference_t<T>&&>(t);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_MOVE_H_