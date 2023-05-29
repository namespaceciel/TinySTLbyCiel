#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_TO_ADDRESS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_TO_ADDRESS_H_

#include <ciel/memory_impl/pointer_traits.h>

namespace ciel {

	template<class T>
	constexpr T* to_address(T* p) noexcept {
		static_assert(!ciel::is_function_v<T>);
		return p;
	}

	template<class T>
	constexpr auto to_address(const T& p) noexcept {
		if constexpr (requires{ ciel::pointer_traits<T>::to_address(p); }) {
			return ciel::pointer_traits<T>::to_address(p);
		} else {
			return ciel::to_address(p.operator->());
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_TO_ADDRESS_H_