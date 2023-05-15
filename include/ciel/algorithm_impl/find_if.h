#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_IF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_IF_H_

namespace ciel {

	template<class InputIt, class UnaryPredicate>
	constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
		for (; first != last; ++first) {
			if (p(*first)) {
				return first;
			}
		}
		return last;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_IF_H_