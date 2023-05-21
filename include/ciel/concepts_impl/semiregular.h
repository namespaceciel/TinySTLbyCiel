#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SEMIREGULAR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SEMIREGULAR_H_

#include <ciel/concepts_impl/copyable.h>
#include <ciel/concepts_impl/default_initializable.h>

namespace ciel {

	template<class T>
	concept semiregular = ciel::copyable<T> && ciel::default_initializable<T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SEMIREGULAR_H_