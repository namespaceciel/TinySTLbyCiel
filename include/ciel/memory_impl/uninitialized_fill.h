#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_FILL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_FILL_H_

#include <ciel/iterator.h>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	template<class ForwardIt, class T>
	void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value) {
		using V = typename ciel::iterator_traits<ForwardIt>::value_type;
		ForwardIt current = first;
		try {
			for (; current != last; ++current) {
				::new(static_cast<void*>(ciel::addressof(*current))) V(value);
			}
		} catch (...) {
			for (; first != current; ++first) {
				first->~V();
			}
			throw;
		}
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_FILL_H_