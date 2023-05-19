#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_MOVE_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_MOVE_N_H_

#include <utility>
#include <ciel/memory_impl/addressof.h>
#include <ciel/memory_impl/destroy.h>

namespace ciel {

	template<class InputIt, class Size, class ForwardIt>
	std::pair<InputIt, ForwardIt> uninitialized_move_n(InputIt first, Size count, ForwardIt d_first) {
		using Value = typename std::iterator_traits<ForwardIt>::value_type;
		ForwardIt current = d_first;
		try {
			for (; count > 0; ++first, (void)++current, --count) {
				::new(const_cast<void*>(static_cast<const volatile void*>(ciel::addressof(*current)))) Value(std::move(*first));
			}
		} catch (...) {
			ciel::destroy(d_first, current);
			throw;
		}
		return {first, current};
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_MOVE_N_H_