#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_APPLY_CV_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_APPLY_CV_H_

//Not C++ standard

#include "is_const.h"
#include "is_volatile.h"
#include "remove_reference.h"

namespace ciel {

	template<class T, class U, bool = is_const_v<remove_reference_t<T>>, bool = is_volatile_v<remove_reference_t<T>>>
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
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_APPLY_CV_H_