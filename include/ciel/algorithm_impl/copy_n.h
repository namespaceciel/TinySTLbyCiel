#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_N_H_

namespace ciel {

	template<class InputIt, class Size, class OutputIt>
	constexpr OutputIt copy_n(InputIt first, Size count, OutputIt result) {
		if (count > 0) {
			*result++ = *first;
			for (Size i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_N_H_