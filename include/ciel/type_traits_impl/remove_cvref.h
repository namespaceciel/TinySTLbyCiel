#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_REMOVE_CVREF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_REMOVE_CVREF_H_

#include <ciel/type_traits_impl/remove_cv.h>
#include <ciel/type_traits_impl/remove_reference.h>

namespace ciel {

	template<class T>
	struct remove_cvref {
		using type = ciel::remove_cv_t<ciel::remove_reference_t<T>>;
	};

	template<class T>
	using remove_cvref_t = typename remove_cvref<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_REMOVE_CVREF_H_