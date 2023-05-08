#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COPY_CV_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COPY_CV_H_

//Not C++ standard

#include <ciel/type_traits_impl/add_cv.h>

namespace ciel {

	template<class From, class To>
	struct copy_cv {
		using type = To;
	};

	template<class From, class To>
	struct copy_cv<const From, To> {
		using type = add_const_t<To>;
	};

	template<class From, class To>
	struct copy_cv<volatile From, To> {
		using type = add_volatile_t<To>;
	};

	template<class From, class To>
	struct copy_cv<const volatile From, To> {
		using type = add_cv_t<To>;
	};

	template<class From, class To>
	using copy_cv_t = typename copy_cv<From, To>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COPY_CV_H_