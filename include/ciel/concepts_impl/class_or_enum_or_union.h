#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CLASS_OR_ENUM_OR_UNION_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CLASS_OR_ENUM_OR_UNION_H_

// Not C++ standard

#include <ciel/type_traits.h>

namespace ciel {

	template<class T>
	concept class_or_enum_or_union = ciel::is_class_v<T> || ciel::is_enum_v<T> || ciel::is_union_v<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CLASS_OR_ENUM_OR_UNION_H_