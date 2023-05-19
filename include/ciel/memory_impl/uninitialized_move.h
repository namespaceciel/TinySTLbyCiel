#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_MOVE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_MOVE_H_

#include <iterator>
#include <ciel/utility.h>
#include <ciel/memory_impl/addressof.h>
#include <ciel/memory_impl/destroy.h>

namespace ciel {

	template<class InputIt, class NoThrowForwardIt>
	NoThrowForwardIt uninitialized_move(InputIt first, InputIt last, NoThrowForwardIt d_first) {
		using Value = typename std::iterator_traits<NoThrowForwardIt>::value_type;
		NoThrowForwardIt current = d_first;
		try {
			for (; first != last; ++first, (void)++current) {
				::new(static_cast<void*>(ciel::addressof(*current))) Value(ciel::move(*first));
			}
			return current;
		} catch (...) {
			ciel::destroy(d_first, current);
			throw;
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_MOVE_H_