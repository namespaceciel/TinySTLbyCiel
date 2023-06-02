#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_MOVABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_MOVABLE_H_

#include <ciel/concepts_impl/move_constructible.h>
#include <ciel/concepts_impl/assignable_from.h>
#include <ciel/concepts_impl/swappable.h>

namespace ciel {

	template<class T>
	concept movable = ciel::is_object_v<T> && ciel::move_constructible<T> && ciel::assignable_from<T&, T> && ciel::swappable<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_MOVABLE_H_