#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_HEAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_HEAP_H_

#include <ciel/algorithm_impl/is_heap_until.h>

namespace ciel {

	template<ciel::legacy_random_access_iterator RandomIt>
	constexpr bool is_heap(RandomIt first, RandomIt last) {
		return is_heap_until_details::is_heap_until_helper(ciel::move(first), last, ciel::less<typename ciel::iterator_traits<RandomIt>::value_type>()) == last;
	}

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr bool is_heap(RandomIt first, RandomIt last, Compare comp) {
		return is_heap_until_details::is_heap_until_helper(ciel::move(first), last, comp) == last;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_HEAP_H_