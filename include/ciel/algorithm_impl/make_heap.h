#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MAKE_HEAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MAKE_HEAP_H_

#include <ciel/algorithm_impl/sift_down.h>
#include <ciel/functional.h>

namespace ciel {

	namespace make_heap_details {
		template<ciel::legacy_random_access_iterator RandomIt, class Compare>
		constexpr void make_heap_helper(RandomIt first, RandomIt last, Compare&& comp) {
			using difference_type = ciel::iterator_traits<RandomIt>::difference_type;
			difference_type len = last - first;
			if (len > 1) {
				for (difference_type offset = (len - 2) / 2; offset >= 0; --offset) {
					ciel::sift_down(first, ciel::forward<Compare>(comp), len, first + offset);
				}
			}
		}
	}

	template<ciel::legacy_random_access_iterator RandomIt>
	constexpr void make_heap(RandomIt first, RandomIt last) {
		make_heap_details::make_heap_helper(ciel::move(first), ciel::move(last), ciel::less<typename ciel::iterator_traits<RandomIt>::value_type>());
	}

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr void make_heap(RandomIt first, RandomIt last, Compare comp) {
		make_heap_details::make_heap_helper(ciel::move(first), ciel::move(last), comp);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MAKE_HEAP_H_