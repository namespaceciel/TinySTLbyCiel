#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ENUM_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ENUM_H_

#include "is_convertible.h"
#include "is_class"

namespace ciel {

	struct int_convertible{ int_convertible(int); };

	template <bool is_arithmetic_or_reference>
	struct is_enum_helper { template <typename T> struct nest : public is_convertible<T, int_convertible>{}; };

	template <>
	struct is_enum_helper<true> { template <typename T> struct nest : public false_type {}; };

	template <typename T>
	struct is_enum_helper2
	{
		typedef type_or<is_arithmetic<T>::value, is_reference<T>::value, is_class<T>::value> selector;
		typedef is_enum_helper<selector::value> helper_t;
		typedef typename add_reference<T>::type ref_t;
		typedef typename helper_t::template nest<ref_t> result;
	};

	template <typename T>
	struct is_enum : public integral_constant<bool, is_enum_helper2<T>::result::value>{};

	template <> struct is_enum<void> : public false_type {};
	template <> struct is_enum<void const> : public false_type {};
	template <> struct is_enum<void volatile> : public false_type {};
	template <> struct is_enum<void const volatile> : public false_type {};
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ENUM_H_