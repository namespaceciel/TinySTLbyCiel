#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_TRAITS_H_

namespace ciel {

	template< class I >
	struct incrementable_traits { };

	template< class T >
	requires std::is_object_v<T>
	struct incrementable_traits<T*>;

	template< class T >
	struct incrementable_traits<const T> : incrementable_traits<T> { };

	template< class T >
	requires requires { typename T::difference_type; }
	struct incrementable_traits<T>;

	template< class T >
	requires (!requires { typename T::difference_type; }) &&
	requires(const T& a, const T& b) { { a - b } -> std::integral; }
struct incrementable_traits<T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_TRAITS_H_