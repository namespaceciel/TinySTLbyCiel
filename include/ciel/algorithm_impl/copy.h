#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_H_

namespace ciel {
	// 若 d_first 在范围 [first, last) 中则行为未定义。此情况下可用 copy_backward 代替
	template<class InputIt, class OutputIt>
	constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = *first++;
		}
		return d_first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_H_