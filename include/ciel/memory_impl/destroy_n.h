#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_N_H_

#include <ciel/memory_impl/destroy_at.h>

namespace ciel {

	template<class ForwardIt, class Size>
	constexpr ForwardIt destroy_n(ForwardIt first, Size n) {
		for (; n > 0; (void)++first, --n) {
			ciel::destroy_at(ciel::addressof(*first));
		}
		return first;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DESTROY_N_H_