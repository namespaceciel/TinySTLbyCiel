#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COPY_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COPY_CONSTRUCTIBLE_H_

#include <ciel/concepts_impl/move_constructible.h>

namespace ciel {

	template<class T>
	concept copy_constructible = ciel::move_constructible<T>
								 && ciel::constructible_from<T, T&>
								 && ciel::convertible_to<T&, T>
								 && ciel::constructible_from<T, const T&>
							 	 && ciel::convertible_to<const T&, T>
						 		 && ciel::constructible_from<T, const T>
								 && ciel::convertible_to<const T, T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COPY_CONSTRUCTIBLE_H_