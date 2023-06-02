#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FILL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FILL_H_

namespace ciel {

	template<class ForwardIt, class T>
	constexpr void fill(ForwardIt first, ForwardIt last, const T& value) {
		for (; first != last; ++first) {
			*first = value;
		}
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FILL_H_