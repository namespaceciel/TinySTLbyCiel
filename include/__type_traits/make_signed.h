#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_MAKE_SIGNED_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_MAKE_SIGNED_H_

#include "is_integral.h"
#include "is_enum.h"
#include "type_list.h"
#include "apply_cv.h"
#include "remove_cv.h"

namespace ciel {

	template<class T, bool = is_integral_v<T> || is_enum_v<T>>
	struct make_signed_helper {};

#define PLACEHOLDER signed long long
	using signed_types = type_list<signed char,
								   type_list<signed short,
											 type_list<signed int,
													   type_list<signed long,
																 type_list<signed long long, PLACEHOLDER >>>>>;
#undef PLACEHOLDER

	template<class T>
	struct make_signed_helper<T, true> {
		using type = typename find_first<signed_types, sizeof(T)>::type;
	};

	template<>
	struct make_signed_helper<bool, true> {};

	template<>
	struct make_signed_helper<signed short, true> {
		using type = signed short;
	};

	template<>
	struct make_signed_helper<unsigned short, true> {
		using type = signed short;
	};

	template<>
	struct make_signed_helper<signed int, true> {
		using type = signed int;
	};

	template<>
	struct make_signed_helper<unsigned int, true> {
		using type = signed int;
	};

	template<>
	struct make_signed_helper<signed long, true> {
		using type = signed long;
	};

	template<>
	struct make_signed_helper<unsigned long, true> {
		using type = signed long;
	};

	template<>
	struct make_signed_helper<signed long long, true> {
		using type = signed long long;
	};

	template<>
	struct make_signed_helper<unsigned long long, true> {
		using type = signed long long;
	};

	template<class T>
	struct make_signed {
		using type = apply_cv_t<T, typename make_signed_helper<remove_cv_t<T>>::type>;
	};

	template<class T>
	using make_signed_t = typename make_signed<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_MAKE_SIGNED_H_