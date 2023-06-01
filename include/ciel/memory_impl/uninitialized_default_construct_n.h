#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_N_H_

#include <ciel/iterator_impl/iterator_traits.h>
#include <ciel/memory_impl/addressof.h>
#include <ciel/memory_impl/destroy.h>

namespace ciel {

	template<class ForwardIt, class Size>
	ForwardIt uninitialized_default_construct_n(ForwardIt first, Size n) {
		using T = typename ciel::iterator_traits<ForwardIt>::value_type;
		ForwardIt current = first;
		try {
			for (; n > 0; (void)++current, --n) {
				::new(const_cast<void*>(static_cast<const volatile void*>(ciel::addressof(*current)))) T;
			}
			return current;
		} catch (...) {
			ciel::destroy(first, current);
			throw;
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_DEFAULT_CONSTRUCT_N_H_