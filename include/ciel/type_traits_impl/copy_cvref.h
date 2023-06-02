#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COPY_CVREF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COPY_CVREF_H_

// Not C++ standard

#include <ciel/type_traits_impl/copy_cv.h>
#include <ciel/type_traits_impl/add_lvalue_reference.h>
#include <ciel/type_traits_impl/add_rvalue_reference.h>

namespace ciel {

	template<class From, class To>
	struct copy_cvref {
		using type = ciel::copy_cv_t<From, To>;
	};

	template<class From, class To>
	struct copy_cvref<From&, To> {
		using type = ciel::add_lvalue_reference_t<ciel::copy_cv_t<From, To>>;
	};

	template<class From, class To>
	struct copy_cvref<From&&, To> {
		using type = ciel::add_rvalue_reference_t<ciel::copy_cv_t<From, To>>;
	};

	template<class From, class To>
	using copy_cvref_t = typename copy_cvref<From, To>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COPY_CVREF_H_