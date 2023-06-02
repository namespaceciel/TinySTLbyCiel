#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DEFAULT_INITIALIZABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DEFAULT_INITIALIZABLE_H_

#include <ciel/concepts_impl/constructible_from.h>

namespace ciel {

	template<class T>
	concept default_initializable = ciel::constructible_from<T>
									&& requires { T{}; }
									&& requires { ::new (static_cast<void*>(nullptr)) T; };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DEFAULT_INITIALIZABLE_H_