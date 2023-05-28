#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_WEAKLY_INCREMENTABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_WEAKLY_INCREMENTABLE_H_

#include <ciel/iterator_impl/iter_alias.h>

namespace ciel {

	template<class I>
	concept weakly_incrementable = ciel::default_initializable<I>
		&& ciel::movable<I>
		&& requires(I i) {
			typename ciel::iter_difference_t<I>;
			requires ciel::signed_integral<ciel::iter_difference_t<I>>;
			{ ++i } -> ciel::same_as<I&>;
			i++;
		};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_WEAKLY_INCREMENTABLE_H_