#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_DISTANCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_DISTANCE_H_

#include <ciel/iterator_impl/iterator_traits.h>

namespace ciel {

	template<class It>
	constexpr typename ciel::iterator_traits<It>::difference_type distance(It first, It last) {
		using category = typename ciel::iterator_traits<It>::iterator_category;
		static_assert(ciel::is_base_of_v<ciel::input_iterator_tag, category>, "ciel::distance 传入的迭代器不满足 input_iterator_tag");

		if constexpr (ciel::is_base_of_v<ciel::random_access_iterator_tag, category>) {
			return last - first;
		} else {
			typename ciel::iterator_traits<It>::difference_type result = 0;
			while (first != last) {
				++first;
				++result;
			}
			return result;
		}
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_DISTANCE_H_