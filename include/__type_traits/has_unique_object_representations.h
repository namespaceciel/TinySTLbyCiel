#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATIONS_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATIONS_H_

#include "integral_constant.h"
#include "remove_cv.h"
#include "remove_all_extents.h"

namespace ciel {

	template<class T>
	struct has_unique_object_representations:bool_constant<__has_unique_object_representations(remove_cv_t<remove_all_extents_t<T>>)>{};

	template< class T >
	inline constexpr bool has_unique_object_representations_v = has_unique_object_representations<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_HAS_UNIQUE_OBJECT_REPRESENTATIONS_H_