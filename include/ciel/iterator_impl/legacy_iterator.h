#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_ITERATOR_H_

#include <ciel/iterator_impl/referenceable.h>
#include <ciel/concepts.h>

namespace ciel {

	template<class I>
	concept legacy_iterator = requires(I i) {
								   { *i } -> ciel::referenceable;
								   { ++i } -> ciel::same_as<I&>;
								   { *i++ } -> ciel::referenceable;
							   }
							&& ciel::copyable<I>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_ITERATOR_H_