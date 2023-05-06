#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_AT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_AT_H_

#include <ciel/memory_impl/addressof.h>
#include <ciel/type_traits.h>

namespace ciel {

	template<class T>
	constexpr void destroy_at(T* p) {
		if constexpr (is_array_v<T>) {
			for (auto& elem : *p) {
				destroy_at(addressof(elem));
			}
		} else {
			p->~T();
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_AT_H_