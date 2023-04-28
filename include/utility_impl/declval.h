#ifndef TINYSTLBYCIEL_INCLUDE_UTILITY_IMPL_DECLVAL_H_
#define TINYSTLBYCIEL_INCLUDE_UTILITY_IMPL_DECLVAL_H_

#include "../type_traits.h"

namespace ciel {

	template<class T>
	std::add_rvalue_reference_t<T> declval() noexcept;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_UTILITY_IMPL_DECLVAL_H_