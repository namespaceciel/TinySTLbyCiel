#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_MAKE_SIGNED_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_MAKE_SIGNED_H_

#include <ciel/type_traits_impl/is_integral.h>
#include <ciel/type_traits_impl/is_enum.h>
#include <ciel/type_traits_impl/type_list.h>
#include <ciel/type_traits_impl/apply_cv.h>
#include <ciel/type_traits_impl/remove_cv.h>

namespace ciel {

	namespace make_signed_details {

		template<class T, bool = ciel::is_integral_v<T> || ciel::is_enum_v<T>>
		struct make_signed_helper {};

#define PLACEHOLDER signed long long
		using signed_types = ciel::type_list<signed char,
											 ciel::type_list<signed short,
															 ciel::type_list<signed int,
																			 ciel::type_list<signed long,
																							 ciel::type_list<signed long long, PLACEHOLDER >>>>>;
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
	}

	template<class T>
	struct make_signed {
		using type = ciel::apply_cv_t<T, typename make_signed_details::make_signed_helper<ciel::remove_cv_t<T>>::type>;
	};

	template<class T>
	using make_signed_t = typename make_signed<T>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_MAKE_SIGNED_H_