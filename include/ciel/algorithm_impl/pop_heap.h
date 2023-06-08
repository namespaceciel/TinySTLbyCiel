#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_POP_HEAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_POP_HEAP_H_

#include <ciel/algorithm_impl/sift_down.h>
#include <ciel/functional.h>

namespace ciel {

	namespace pop_heap_details {
		template<ciel::legacy_random_access_iterator RandomIt, class Compare>
		constexpr void pop_heap_helper(RandomIt first, RandomIt last, Compare&& comp) {
			ciel::swap(*first, *(last - 1));
			ciel::sift_down(first, ciel::forward<Compare>(comp), last - first - 1, first);
		}
	}

	template<ciel::legacy_random_access_iterator RandomIt>
	constexpr void pop_heap(RandomIt first, RandomIt last) {
		pop_heap_details::pop_heap_helper(ciel::move(first), ciel::move(last), ciel::less<typename ciel::iterator_traits<RandomIt>::value_type>());
	}

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr void pop_heap(RandomIt first, RandomIt last, Compare comp) {
		pop_heap_details::pop_heap_helper(ciel::move(first), ciel::move(last), comp);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_POP_HEAP_H_