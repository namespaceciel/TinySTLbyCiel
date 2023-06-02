#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_IF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_IF_H_

namespace ciel {
	// 若源与目标范围重叠则行为未定义
	template<class InputIt, class OutputIt, class UnaryPredicate>
	constexpr OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred) {
		while (first != last) {
			if (pred(*first)) {
				*d_first++ = *first;
			}
			++first;
		}
		return d_first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_IF_H_