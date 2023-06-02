#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_EXCHANGE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_EXCHANGE_H_

#include <ciel/type_traits_impl/is_move_constructible.h>
#include <ciel/type_traits_impl/is_assignable.h>
#include <ciel/utility_impl/move.h>
#include <ciel/utility_impl/forward.h>

namespace ciel {

	template<class T, class U = T>
	constexpr T exchange(T& obj, U&& new_value) noexcept(ciel::is_nothrow_move_constructible_v<T> && ciel::is_nothrow_assignable_v<T&, U>) {
		T old_value = ciel::move(obj);
		obj = ciel::forward<U>(new_value);
		return old_value;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_EXCHANGE_H_