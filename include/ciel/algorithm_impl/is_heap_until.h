#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_HEAP_UNTIL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_HEAP_UNTIL_H_

#include <ciel/iterator.h>

namespace ciel {

	namespace is_heap_until_details {
		template<ciel::legacy_random_access_iterator RandomIt, class Compare>
		constexpr RandomIt is_heap_until_helper(RandomIt first, RandomIt last, Compare&& comp) {
			RandomIt res = first;
			while (++res < last) {
				if (comp(*(first + (res - first - 1) / 2), *res)) {
					return res;
				}
			}
			return res;
		}
	}

	template<ciel::legacy_random_access_iterator RandomIt>
	constexpr RandomIt is_heap_until(RandomIt first, RandomIt last) {
		return is_heap_until_details::is_heap_until_helper(ciel::move(first), ciel::move(last), ciel::less<typename ciel::iterator_traits<RandomIt>::value_type>());
	}

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr RandomIt is_heap_until(RandomIt first, RandomIt last, Compare comp) {
		return is_heap_until_details::is_heap_until_helper(ciel::move(first), ciel::move(last), comp);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_HEAP_UNTIL_H_