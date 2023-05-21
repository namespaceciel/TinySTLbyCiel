#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_READABLE_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_READABLE_TRAITS_H_

#include <ciel/concepts.h>
#include <ciel/iterator_impl/has_member_value_type.h>
#include <ciel/iterator_impl/has_member_element_type.h>

namespace ciel {

	template<class I>
	struct indirectly_readable_traits {};

	template<class T>
	struct indirectly_readable_traits<T*> {};

	template<class T>
		requires ciel::is_object_v<T>
	struct indirectly_readable_traits<T*> {
		using value_type = ciel::remove_cv_t<T>;
	};

	template<class I>
		requires ciel::is_array_v<I>
	struct indirectly_readable_traits<I> {
		using value_type = ciel::remove_cv_t<ciel::remove_extent_t<I>>;
	};

	template<class T>
	struct indirectly_readable_traits<const T> : indirectly_readable_traits<T> {};

	template<ciel::has_member_value_type T>
	struct indirectly_readable_traits<T> {};

	template<ciel::has_member_value_type T>
		requires ciel::is_object_v<typename T::value_type>
	struct indirectly_readable_traits<T> {
		using value_type = ciel::remove_cv_t<typename T::value_type>;
	};

	template<ciel::has_member_element_type T>
	struct indirectly_readable_traits<T> {};

	template<ciel::has_member_element_type T>
		requires ciel::is_object_v<typename T::element_type>
	struct indirectly_readable_traits<T> {
		using value_type = ciel::remove_cv_t<typename T::element_type>;
	};

	template<ciel::has_member_value_type T>
		requires ciel::has_member_element_type<T>
	struct indirectly_readable_traits<T> {};

	template<ciel::has_member_value_type T>
		requires ciel::has_member_element_type<T> && ciel::is_object_v<typename T::value_type> && ciel::same_as<ciel::remove_cv_t<typename T::element_type>, ciel::remove_cv_t<typename T::value_type>>
	struct indirectly_readable_traits<T> {
		using value_type = ciel::remove_cv_t<typename T::value_type>;
	};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_READABLE_TRAITS_H_