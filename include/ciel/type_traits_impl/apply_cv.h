#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_APPLY_CV_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_APPLY_CV_H_

// Not C++ standard

#include <ciel/type_traits_impl/is_const.h>
#include <ciel/type_traits_impl/is_volatile.h>
#include <ciel/type_traits_impl/remove_reference.h>

namespace ciel {

	template<class T, class U, bool = ciel::is_const_v<ciel::remove_reference_t<T>>, bool = ciel::is_volatile_v<ciel::remove_reference_t<T>>>
	struct apply_cv {
		using type = U;
	};

	template<class T, class U>
	struct apply_cv<T, U, true, true> {
		using type = const volatile U;
	};

	template<class T, class U>
	struct apply_cv<T, U, true, false> {
		using type = const U;
	};

	template<class T, class U>
	struct apply_cv<T, U, false, true> {
		using type = volatile U;
	};

	template<class T, class U>
	struct apply_cv<T&, U, false, false> {
		using type = U&;
	};

	template<class T, class U>
	struct apply_cv<T&, U, true, true> {
		using type = const volatile U&;
	};

	template<class T, class U>
	struct apply_cv<T&, U, true, false> {
		using type = const U&;
	};

	template<class T, class U>
	struct apply_cv<T&, U, false, true> {
		using type = volatile U&;
	};

	template<class T, class U>
	using apply_cv_t = typename apply_cv<T, U>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_APPLY_CV_H_