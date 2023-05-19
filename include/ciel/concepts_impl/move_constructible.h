#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_MOVE_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_MOVE_CONSTRUCTIBLE_H_

#include <ciel/concepts_impl/constructible_from.h>
#include <ciel/concepts_impl/convertible_to.h>

namespace ciel {

	template<class T>
	concept move_constructible = ciel::constructible_from<T, T> && ciel::convertible_to<T, T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_MOVE_CONSTRUCTIBLE_H_