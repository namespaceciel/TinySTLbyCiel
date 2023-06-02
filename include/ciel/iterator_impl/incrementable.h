#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_H_

#include <ciel/iterator_impl/weakly_incrementable.h>

namespace ciel {

	template<class I>
	concept incrementable = ciel::regular<I>
		&& ciel::weakly_incrementable<I>
		&& requires(I i) {
			{ i++ } -> ciel::same_as<I>;
		};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_H_