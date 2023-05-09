#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TRAITS_H_

namespace ciel {

	template< class Iter >
	struct iterator_traits;
	template< class T >
	struct iterator_traits<T*>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TRAITS_H_