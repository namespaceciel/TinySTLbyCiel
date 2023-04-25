#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_CVREF_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_CVREF_H_

#include "remove_cv.h"
#include "remove_reference.h"

namespace ciel {

	template<class T>
	struct remove_cvref {
		typedef remove_cv_t<remove_reference_t<T>> type;
	};

	template<class T>
	using remove_cvref_t = typename remove_cvref<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_CVREF_H_