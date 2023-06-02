#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_HAS_UNIQUE_OBJECT_REPRESENTATIONS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_HAS_UNIQUE_OBJECT_REPRESENTATIONS_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/remove_cv.h>
#include <ciel/type_traits_impl/remove_all_extents.h>

namespace ciel {

	template<class T>
	struct has_unique_object_representations : ciel::bool_constant<__has_unique_object_representations(ciel::remove_cv_t<ciel::remove_all_extents_t<T>>)> {};

	template<class T>
	inline constexpr bool has_unique_object_representations_v = has_unique_object_representations<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_HAS_UNIQUE_OBJECT_REPRESENTATIONS_H_