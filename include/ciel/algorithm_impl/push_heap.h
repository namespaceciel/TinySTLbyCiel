#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_PUSH_HEAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_PUSH_HEAP_H_

#include <ciel/iterator.h>

namespace ciel {

	// 注意：待 push 元素本身已经存在于下标 last - 1 处

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr void sift_up(RandomIt first, RandomIt last, Compare&& comp, typename ciel::iterator_traits<RandomIt>::difference_type len) {
		using value_type = ciel::iterator_traits<RandomIt>::value_type;
		if (len > 1) {
			len = (len - 2) / 2;
			RandomIt head = first + len;
			if (comp(*head, *--last)) {    // 如果头结点小于自己，需要上浮
				value_type tmp_larger = ciel::move(*last);    // 目前最大的元素，需要一直上浮
				do {
					*last = ciel::move(*head);
					last = head;
					if (len == 0) {
						break;
					}
					len = (len - 1) / 2;    // 注意是 - 1
					head = first + len;
				} while (comp(*head, tmp_larger));
				*last = ciel::move(tmp_larger);
			}
		}
	}

	namespace push_heap_details {
		template<ciel::legacy_random_access_iterator RandomIt, class Compare>
		constexpr void push_heap_helper(RandomIt first, RandomIt last, Compare&& comp) {
			typename ciel::iterator_traits<RandomIt>::difference_type len = last - first;
			sift_up(ciel::move(first), ciel::move(last), ciel::forward<Compare>(comp), len);
		}
	}

	template<ciel::legacy_random_access_iterator RandomIt>
	constexpr void push_heap(RandomIt first, RandomIt last) {
		push_heap_details::push_heap_helper(ciel::move(first), ciel::move(last), ciel::less<typename ciel::iterator_traits<RandomIt>::value_type>());
	}

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr void push_heap(RandomIt first, RandomIt last, Compare comp) {
		push_heap_details::push_heap_helper(ciel::move(first), ciel::move(last), comp);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_PUSH_HEAP_H_