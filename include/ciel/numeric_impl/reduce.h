#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_REDUCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_REDUCE_H_

#include <ciel/iterator.h>
#include <ciel/functional.h>

namespace ciel {

	// reduce 应该有更多的性能优化手段，最差情况会跟 accumulate 相等，不过标准库也写的是最差情况咱就也不管了

	template<ciel::legacy_input_iterator InputIt>
	constexpr typename ciel::iterator_traits<InputIt>::value_type reduce(InputIt first, InputIt last) {
		return reduce(first, last, typename ciel::iterator_traits<InputIt>::value_type{});
	}

	template<ciel::legacy_input_iterator InputIt, class T>
	constexpr T reduce(InputIt first, InputIt last, T init) {
		return reduce(first, last, init, ciel::plus<>());
	}

	template<ciel::legacy_input_iterator InputIt, class T, class BinaryOp>
	constexpr T reduce(InputIt first, InputIt last, T init, BinaryOp binary_op) {
		for (; first != last; ++first) {
			init = binary_op(init, *first);
		}
		return init;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_REDUCE_H_