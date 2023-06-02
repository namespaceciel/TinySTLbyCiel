#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_REFERENCEABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_REFERENCEABLE_H_

#include <ciel/utility.h>

namespace ciel {

	template<class T>
	using with_reference = T&;

	template<class T>
	concept can_reference = requires { typename with_reference<T>; };

	// 若且唯若表达式 *declval<T&>() 合法且拥有可引用类型（特别是非 void ）才得到满足。
	template<class T>
	concept referenceable = requires {{ *ciel::declval<T&>() } -> can_reference; };

	template<referenceable T>
	using iter_reference_t = decltype(*ciel::declval<T&>());

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_REFERENCEABLE_H_