#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_DECLVAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_DECLVAL_H_

#include "../type_traits_impl/add_rvalue_reference.h"

namespace ciel {

	template<class T>
	add_rvalue_reference_t<T> declval() noexcept;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_DECLVAL_H_