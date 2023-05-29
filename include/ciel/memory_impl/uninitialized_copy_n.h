#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_COPY_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_COPY_N_H_

#include <ciel/iterator.h>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	template<class InputIt, class Size, class NoThrowForwardIt>
	NoThrowForwardIt uninitialized_copy_n(InputIt first, Size count, NoThrowForwardIt d_first) {
		using T = typename ciel::iterator_traits<NoThrowForwardIt>::value_type;
		NoThrowForwardIt current = d_first;
		try {
			for (; count > 0; ++first, (void)++current, --count) {
				::new(static_cast<void*>(ciel::addressof(*current))) T(*first);
			}
		} catch (...) {
			for (; d_first != current; ++d_first) {
				d_first->~T();
			}
			throw;
		}
		return current;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_COPY_N_H_