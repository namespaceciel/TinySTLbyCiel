#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_GENERATE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_GENERATE_H_

namespace ciel {

	template<class ForwardIt, class Generator>
	constexpr void generate(ForwardIt first, ForwardIt last, Generator g) {
		while (first != last) {
			*first++ = g();
		}
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_GENERATE_H_