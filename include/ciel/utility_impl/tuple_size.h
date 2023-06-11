#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_TUPLE_SIZE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_TUPLE_SIZE_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <cstddef>

namespace ciel {

	template<class T>
	struct tuple_size;

	template<class T>
	struct tuple_size<const T> : ciel::integral_constant<size_t, tuple_size<T>::value> {};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_TUPLE_SIZE_H_