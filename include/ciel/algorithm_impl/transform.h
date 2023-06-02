#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_TRANSFORM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_TRANSFORM_H_

namespace ciel {

	template<class InputIt, class OutputIt, class UnaryOperation>
	constexpr OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
		while (first1 != last1) {
			*d_first++ = unary_op(*first1++);
		}
		return d_first;
	}

	template<class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
	constexpr OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first, BinaryOperation binary_op) {
		while (first1 != last1) {
			*d_first++ = binary_op(*first1++, *first2++);
		}
		return d_first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_TRANSFORM_H_