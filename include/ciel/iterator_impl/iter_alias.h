#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_ALIAS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_ALIAS_H_

#include <ciel/concepts.h>
#include <ciel/iterator_impl/referenceable.h>    // iter_reference_t
#include <ciel/iterator_impl/indirectly_readable_traits.h>
#include <ciel/iterator_impl/iterator_traits.h>
#include <ciel/iterator_impl/ranges::iter_move.h>

namespace ciel {

	namespace iter_alias_details {
		template<class T, class = void>
		struct iter_value_t_helper {
			using value_type = typename ciel::indirectly_readable_traits<ciel::remove_cvref_t<T>>::value_type;
		};

		template<class T>
		struct iter_value_t_helper<T, ciel::void_t<typename ciel::iterator_traits<ciel::remove_cvref_t<T>>::value_type>> {
			using value_type = typename ciel::iterator_traits<ciel::remove_cvref_t<T>>::value_type;
		};

		template<class T, class = void>
		struct iter_difference_t_helper {
			using value_type = typename ciel::incrementable_traits<ciel::remove_cvref_t<T>>::difference_type;
		};

		template<class T>
		struct iter_difference_t_helper<T, ciel::void_t<typename ciel::iterator_traits<ciel::remove_cvref_t<T>>::difference_type>> {
			using value_type = typename ciel::iterator_traits<ciel::remove_cvref_t<T>>::difference_type;
		};
	}

	template<class T>
	using iter_value_t = typename iter_alias_details::iter_value_t_helper<T>::value_type;

	template<class T>
	using iter_difference_t = typename iter_alias_details::iter_difference_t_helper<T>::value_type;

	template<referenceable T>
		requires requires {{ ciel::ranges::iter_move(ciel::declval<T&>()) } -> can_reference; }
	using iter_rvalue_reference_t = decltype(ciel::ranges::iter_move(ciel::declval<T&>()));

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_ALIAS_H_