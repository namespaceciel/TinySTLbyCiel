#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_REGULAR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_REGULAR_H_

#include <ciel/concepts_impl/semiregular.h>
#include <ciel/concepts_impl/equality_comparable.h>

namespace ciel {

	template<class T>
	concept regular = ciel::semiregular<T> && ciel::equality_comparable<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_REGULAR_H_