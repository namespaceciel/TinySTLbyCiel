#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_H_

#include <ciel/memory_impl/destroy_at.h>

namespace ciel {

	template<class ForwardIt>
	constexpr void destroy(ForwardIt first, ForwardIt last) {
		for (; first != last; ++first) {
			ciel::destroy_at(ciel::addressof(*first));
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_H_