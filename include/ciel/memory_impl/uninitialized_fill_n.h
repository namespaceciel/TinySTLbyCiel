#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_FILL_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_FILL_N_H_

#include <iterator>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	template<class ForwardIt, class Size, class T>
	ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value) {
		using V = typename std::iterator_traits<ForwardIt>::value_type;
		ForwardIt current = first;
		try {
			for (; count > 0; ++current, (void)--count) {
				::new(static_cast<void*>(ciel::addressof(*current))) V(value);
			}
			return current;
		} catch (...) {
			for (; first != current; ++first) {
				first->~V();
			}
			throw;
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_FILL_N_H_