#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_ACCUMULATE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_ACCUMULATE_H_

#include <ciel/utility.h>
#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_input_iterator InputIt, class T>
	constexpr T accumulate(InputIt first, InputIt last, T init) {
		for (; first != last; ++first) {
			init = ciel::move(init) + *first;
		}
		return init;
	}

	template<ciel::legacy_input_iterator InputIt, class T, class BinaryOperation>
	constexpr T accumulate(InputIt first, InputIt last, T init, BinaryOperation op) {
		for (; first != last; ++first) {
			init = op(ciel::move(init), *first);
		}
		return init;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_ACCUMULATE_H_