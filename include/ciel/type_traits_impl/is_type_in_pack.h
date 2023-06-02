#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TYPE_IN_PACK_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TYPE_IN_PACK_H_

// Not C++ standard

#include <ciel/concepts_impl/same_as.h>

namespace ciel {

	template<class Target, class... T>
	struct is_type_in_pack;

	template<class Target>
	struct is_type_in_pack<Target> : ciel::false_type {};

	template<class Target, class Front, class... Rest>
		requires ciel::same_as<Target, Front>
	struct is_type_in_pack<Target, Front, Rest...> : ciel::true_type {};

	template<class Target, class Front, class... Rest>
		requires (!ciel::same_as<Target, Front>)
	struct is_type_in_pack<Target, Front, Rest...> : is_type_in_pack<Target, Rest...> {};

	template<class Target, class... T>
	inline constexpr bool is_type_in_pack_v = is_type_in_pack<Target, T...>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_TYPE_IN_PACK_H_