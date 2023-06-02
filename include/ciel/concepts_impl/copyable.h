#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COPYABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COPYABLE_H_

#include <ciel/concepts_impl/copy_constructible.h>
#include <ciel/concepts_impl/movable.h>

namespace ciel {

	template<class T>
	concept copyable = ciel::copy_constructible<T>
					&& ciel::movable<T>
					&& ciel::assignable_from<T&, T&>
					&& ciel::assignable_from<T&, const T&>
					&& ciel::assignable_from<T&, const T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COPYABLE_H_