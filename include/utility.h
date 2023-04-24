#ifndef TINYSTLBYCIEL_INCLUDE_UTILITY_H_
#define TINYSTLBYCIEL_INCLUDE_UTILITY_H_

#include "type_traits.h"

namespace ciel {

	//move
	template<class T>
	constexpr remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<remove_reference_t<T>&&>(t);
	}

	//forward
	template<class T>
	constexpr T&& forward(remove_reference_t<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	template<class T>
	constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
		return static_cast<T&&>(t);
	}

	//declval
	template<class T>
	typename add_rvalue_reference<T>::type declval() noexcept;

	//pair
	template<class T1, class T2>
	struct pair {
		using first_type = T1;
		using second_type = T2;

		T1 first;
		T2 second;

		constexpr pair();

	};

	template<class T1, class T2>
	constexpr pair<decay_t<T1>, decay_t<T2>> make_pair(T1&& t, T2&& u) {
		return pair<decay_t<T1>, decay_t<T2>>{t,u};
	}
}    //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_UTILITY_H_