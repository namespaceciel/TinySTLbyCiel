#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SORT_HEAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SORT_HEAP_H_

#include <ciel/algorithm_impl/pop_heap.h>

namespace ciel {

	namespace sort_heap_details {
		template<ciel::legacy_random_access_iterator RandomIt, class Compare>
		constexpr void sort_heap_helper(RandomIt first, RandomIt last, Compare&& comp) {
			while (last != first) {
				ciel::pop_heap(first, last, comp);
				--last;
			}
		}
	}

	template<ciel::legacy_random_access_iterator RandomIt>
	constexpr void sort_heap(RandomIt first, RandomIt last) {
		sort_heap_details::sort_heap_helper(ciel::move(first), ciel::move(last), ciel::less<typename ciel::iterator_traits<RandomIt>::value_type>());
	}

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr void sort_heap(RandomIt first, RandomIt last, Compare comp) {
		sort_heap_details::sort_heap_helper(ciel::move(first), ciel::move(last), comp);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SORT_HEAP_H_