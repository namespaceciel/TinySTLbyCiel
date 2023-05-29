#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_ITERATOR_H_

#include <ciel/iterator_impl/referenceable.h>
#include <ciel/concepts.h>

namespace ciel {

	template<class I>
	concept legacy_iterator = requires(I i) {
		{ *i } -> ciel::can_reference;    //这里 cppreference 写的是 __Referenceable<T> 但是与 iter_alias 那里的同名阐述不是一个对象，很坑
		{ ++i } -> ciel::same_as<I&>;
		{ *i++ } -> ciel::can_reference;
	}
		&& ciel::copyable<I>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_ITERATOR_H_