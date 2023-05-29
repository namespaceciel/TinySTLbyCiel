#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ADVANCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ADVANCE_H_

#include <ciel/iterator_impl/iterator_traits.h>

namespace ciel {

	template<class It, class Distance>
	constexpr void advance(It& it, Distance n) {
		using category = typename ciel::iterator_traits<It>::iterator_category;
		static_assert(ciel::is_base_of_v<ciel::input_iterator_tag, category>, "ciel::advance 传入的迭代器不满足 input_iterator_tag");

		auto dist = typename ciel::iterator_traits<It>::difference_type(n);
		if constexpr (ciel::is_base_of_v<ciel::random_access_iterator_tag, category>) {
			it += dist;
		} else {
			while (dist > 0) {
				--dist;
				++it;
			}
			if constexpr (ciel::is_base_of_v<ciel::bidirectional_iterator_tag, category>) {    //n < 0 时
				while (dist < 0) {
					++dist;
					--it;
				}
			}
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ADVANCE_H_