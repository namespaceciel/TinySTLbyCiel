#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TRAITS_H_

#include <ciel/iterator_impl/legacy_random_access_iterator.h>
#include <ciel/iterator_impl/iterator_tag.h>

namespace ciel {

	namespace iterator_traits_details {

		template<class Iter, class = void>
		struct has_pointer1 {
			using type = void;
		};

		template<class Iter>
		struct has_pointer1<Iter, ciel::void_t<typename Iter::pointer>> {
			using type = Iter::pointer;
		};

		// 对应三种情况
		template<class Iter> struct has_pointer2_helper3 { using type = void; };
		template<class Iter> struct has_pointer2_helper2 : has_pointer2_helper3<Iter> {};
		template<class Iter> struct has_pointer2_helper1 : has_pointer2_helper2<Iter> {};

		template<class Iter>
			requires requires { typename Iter::pointer; }
		struct has_pointer2_helper1<Iter> {
			using type = Iter::pointer;
		};

		template<class Iter>
			requires requires { typename ciel::void_t<decltype(ciel::declval<Iter&>().operator->())>; }
		struct has_pointer2_helper2<Iter> {
			using type = decltype(ciel::declval<Iter&>().operator->());
		};

		template<class Iter, class = void>
		struct has_reference2 {
			using type = ciel::iter_reference_t<Iter>;
		};

		template<class Iter>
		struct has_reference2<Iter, ciel::void_t<typename Iter::reference>> {
			using type = Iter::reference;
		};

		// 对应五种情况
		template<class Iter> struct has_iterator_category2_helper5 { using type = ciel::input_iterator_tag; };
		template<class Iter> struct has_iterator_category2_helper4 : has_iterator_category2_helper5<Iter> {};
		template<class Iter> struct has_iterator_category2_helper3 : has_iterator_category2_helper4<Iter> {};
		template<class Iter> struct has_iterator_category2_helper2 : has_iterator_category2_helper3<Iter> {};
		template<class Iter> struct has_iterator_category2_helper1 : has_iterator_category2_helper2<Iter> {};

		template<class Iter>
			requires requires { typename Iter::iterator_category; }
		struct has_iterator_category2_helper1<Iter> {
			using type = Iter::iterator_category;
		};

		template<legacy_random_access_iterator Iter>
		struct has_iterator_category2_helper2<Iter> {
			using type = ciel::random_access_iterator_tag;
		};

		template<legacy_bidirectional_iterator Iter>
		struct has_iterator_category2_helper3<Iter> {
			using type = ciel::bidirectional_iterator_tag;
		};

		template<legacy_forward_iterator Iter>
		struct has_iterator_category2_helper4<Iter> {
			using type = ciel::forward_iterator_tag;
		};

		template<class Iter, class = void>
		struct has_difference_type3 {
			using type = void;
		};

		template<class Iter>
		struct has_difference_type3<Iter, ciel::void_t<typename ciel::incrementable_traits<Iter>::difference_type>> {
			using type = ciel::incrementable_traits<Iter>::difference_type;
		};

		template<class Iter>
		concept has_member_pointer = requires { typename Iter::pointer; };

		template<class Iter>
		concept has_other_four_members = requires {
			typename Iter::difference_type;
			typename Iter::value_type;
			typename Iter::reference;
			typename Iter::iterator_category;
		};

		// 三种存在情况
		template<class Iter> struct iterator_traits_helper3 {};
		template<class Iter> struct iterator_traits_helper2 : iterator_traits_helper3<Iter> {};
		template<class Iter> struct iterator_traits_helper1 : iterator_traits_helper2<Iter> {};

		template<has_other_four_members Iter>
		struct iterator_traits_helper1<Iter> {
			using difference_type = Iter::difference_type;
			using value_type = Iter::value_type;
			using pointer = has_pointer1<Iter>::type;
			using reference = Iter::reference;
			using iterator_category = Iter::iterator_category;
		};

		template<legacy_input_iterator Iter>
		struct iterator_traits_helper2<Iter> {
			using difference_type = ciel::incrementable_traits<Iter>::difference_type;
			using value_type = ciel::indirectly_readable_traits<Iter>::value_type;
			using pointer = has_pointer2_helper1<Iter>::type;
			using reference = has_reference2<Iter>::type;
			using iterator_category = has_iterator_category2_helper1<Iter>::type;
		};

		template<legacy_iterator Iter>
		struct iterator_traits_helper3<Iter> {
			using difference_type = has_difference_type3<Iter>::type;
			using value_type = void;
			using pointer = void;
			using reference = void;
			using iterator_category = ciel::output_iterator_tag;
		};

	}	// namespace iterator_traits_details

	template<class Iter>
	struct iterator_traits : iterator_traits_details::iterator_traits_helper1<Iter> {
		using primary_template = iterator_traits;	// 用于 iter_concept
	};

	template<class T>
		requires ciel::is_object_v<T>
	struct iterator_traits<T*> {
		using difference_type = ptrdiff_t;
		using value_type = ciel::remove_cv_t<T>;
		using pointer = T*;
		using reference = T&;
		using iterator_category = ciel::random_access_iterator_tag;
		using iterator_concept = ciel::contiguous_iterator_tag;
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TRAITS_H_