#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ALIGNMENT_OF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ALIGNMENT_OF_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <cstddef>

namespace ciel {

	template<class T>
	struct alignment_of : integral_constant<size_t, alignof(T)> {};

	template<class T>
	inline constexpr size_t alignment_of_v = alignment_of<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ALIGNMENT_OF_H_