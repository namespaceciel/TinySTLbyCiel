#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_ALIAS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_ALIAS_H_

#include <ciel/concepts.h>
#include <ciel/iterator_impl/referenceable.h>

namespace ciel {



	//若 iterator_traits<remove_cvref_t<T>> 未被特化，则 iter_value_t<T> 为 indirectly_readable_traits<remove_cvref_t<T>>::value_type
	//否则它是 iterator_traits<remove_cvref_t<T>>::value_type 。
	template< class T >
	using iter_value_t = /* see below */;



	template< class T >
	using iter_difference_t = /* see below */;

	template< referenceable T>
		requires /* see below */
	using iter_rvalue_reference_t = decltype(std::iter_move(std::declval<T&>()));

	template< std::indirectly_readable T >
	using iter_common_reference_t = std::common_reference_t<std::iter_reference_t<T>,
	std::iter_value_t<T>&>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_ALIAS_H_