#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_WRITABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_WRITABLE_H_

#include <ciel/iterator_impl/referenceable.h>

namespace ciel {

	template<class Out, class T>
	concept indirectly_writable = requires(Out&& o, T&& t) {
		*o = ciel::forward<T>(t);
		*ciel::forward<Out>(o) = ciel::forward<T>(t);
		const_cast<const ciel::iter_reference_t<Out>&&>(*o) = ciel::forward<T>(t);
		const_cast<const ciel::iter_reference_t<Out>&&>(*ciel::forward<Out>(o)) = ciel::forward<T>(t);
	};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_WRITABLE_H_