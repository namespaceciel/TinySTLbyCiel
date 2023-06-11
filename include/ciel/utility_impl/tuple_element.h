#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_TUPLE_ELEMENT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_TUPLE_ELEMENT_H_

#include <ciel/type_traits_impl/add_cv.h>
#include <cstddef>

namespace ciel {

	template<size_t I, class T>
	struct tuple_element;

	template<size_t I, class T>
	struct tuple_element<I, const T> {
		using type = ciel::add_const_t<typename tuple_element<I, T>::type>;
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_TUPLE_ELEMENT_H_