#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_PAIR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_PAIR_H_

#include <ciel/type_traits.h>
#include <ciel/utility_impl/forward.h>
#include <ciel/utility_impl/move.h>
#include <ciel/utility_impl/swap.h>
#include <functional>

namespace ciel {

	template<class T1, class T2>
	struct pair {

		using first_type = T1;
		using second_type = T2;

		T1 first;
		T2 second;

		constexpr explicit(!ciel::is_implicitly_default_constructible_v<T1> || !ciel::is_implicitly_default_constructible_v<T2>)
		pair() requires ciel::is_default_constructible_v<T1> && ciel::is_default_constructible_v<T2>: first(), second() {}

		constexpr explicit(!ciel::is_convertible_v<const T1&, T1> || !ciel::is_convertible_v<const T2&, T2>)
		pair(const T1& x, const T2& y) requires ciel::is_copy_constructible_v<T1> && ciel::is_copy_constructible_v<T2>: first(x), second(y) {}

		//TODO: 如果 first 或 second 的初始化会绑定引用到临时对象，那么此构造函数会定义为被弃置。(C++23 起)
		template<class U1 = T1, class U2 = T2, class = ciel::enable_if_t<ciel::is_constructible_v<T1, U1> && ciel::is_constructible_v<T2, U2>>>
		constexpr explicit(!ciel::is_convertible_v<U1, T1> || !ciel::is_convertible_v<U2, T2>)
		pair(U1&& x, U2&& y) : first(ciel::forward<U1>(x)), second(ciel::forward<U2>(y)) {}

		//TODO: 如果 first 或 second 的初始化会绑定引用到临时对象，那么此构造函数会定义为被弃置。(C++23 起)
		template<class U1, class U2, class = ciel::enable_if_t<ciel::is_constructible_v<T1, U1&> && ciel::is_constructible_v<T2, U2&>>>
		constexpr explicit(!ciel::is_convertible_v<U1&, T1> || !ciel::is_convertible_v<U2&, T2>)
		pair(pair<U1, U2>& p) : first(p.first), second(p.second) {}

		//TODO: 如果 first 或 second 的初始化会绑定引用到临时对象，那么此构造函数会定义为被弃置。(C++23 起)
		template<class U1, class U2, class = ciel::enable_if_t<ciel::is_constructible_v<T1, const U1&> && ciel::is_constructible_v<T2, const U2&>>>
		constexpr explicit(!ciel::is_convertible_v<const U1&, T1> || !ciel::is_convertible_v<const U2&, T2>)
		pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

		//TODO: 如果 first 或 second 的初始化会绑定引用到临时对象，那么此构造函数会定义为被弃置。(C++23 起)
		template<class U1, class U2, class = ciel::enable_if_t<ciel::is_constructible_v<T1, U1> && ciel::is_constructible_v<T2, U2>>>
		constexpr explicit(!ciel::is_convertible_v<U1, T1> || !ciel::is_convertible_v<U2, T2>)
		pair(pair<U1, U2>&& p) : first(ciel::forward<U1>(p.first)), second(ciel::forward<U2>(p.second)) {}

		//TODO: 如果 first 或 second 的初始化会绑定引用到临时对象，那么此构造函数会定义为被弃置。(C++23 起)
		template<class U1, class U2, class = ciel::enable_if_t<ciel::is_constructible_v<T1, const U1> && ciel::is_constructible_v<T2, const U2>>>
		constexpr explicit(!ciel::is_convertible_v<const U1, T1> || !ciel::is_convertible_v<const U2, T2>)
		pair(const pair<U1, U2>&& p) : first(ciel::forward<const U1>(p.first)), second(ciel::forward<const U2>(p.second)) {}

		//TODO: 如果 first 或 second 的初始化会绑定引用到临时对象，那么此构造函数会定义为被弃置。(C++23 起)
//		template<pair-like P>
//		constexpr pair(P&& u);

//		template<class... Args1, class... Args2>
//		constexpr pair(std::piecewise_construct_t, std::tuple<Args1...> first_args, std::tuple<Args2...> second_args);

		constexpr pair(const pair& p) = default;

		constexpr pair(pair&& p) = default;

		constexpr pair& operator=(const pair& other) requires ciel::is_copy_assignable_v<T1> && ciel::is_copy_assignable_v<T2> {
			first = other.first;
			second = other.second;
			return *this;
		}

		//当 const T1 与 const T2 可以拷贝赋值时，此函数才会启用。比如 T1 与 T2 是空对象或者所有成员变量皆为 mutable 时，pair<T1, T2> 即可在 const 限定下进行拷贝赋值
		constexpr const pair& operator=(const pair& other) const requires ciel::is_copy_assignable_v<const T1> && ciel::is_copy_assignable_v<const T2> {
			first = other.first;
			second = other.second;
			return *this;
		}

		template<class U1, class U2>
		constexpr pair& operator=(const pair<U1, U2>& other) requires ciel::is_assignable_v<T1&, const U1&> && ciel::is_assignable_v<T2&, const U2&> {
			first = other.first;
			second = other.second;
			return *this;
		}

		template<class U1, class U2>
		constexpr const pair& operator=(const pair<U1, U2>& other) const requires ciel::is_assignable_v<const T1&, const U1&> && ciel::is_assignable_v<const T2&, const U2&> {
			first = other.first;
			second = other.second;
			return *this;
		}

		constexpr pair& operator=(pair&& other) noexcept(ciel::is_nothrow_move_assignable_v<T1> && ciel::is_nothrow_move_assignable_v<T2>) requires ciel::is_move_assignable_v<T1> && ciel::is_move_assignable_v<T2> {
			first = ciel::move(other.first);
			second = ciel::move(other.second);
			return *this;
		}

		constexpr const pair& operator=(pair&& other) const requires ciel::is_assignable_v<const T1&, T1> && ciel::is_assignable_v<const T2&, T2> {
			first = ciel::move(other.first);
			second = ciel::move(other.second);
			return *this;
		}

		template<class U1, class U2>
		constexpr pair& operator=(pair<U1, U2>&& other) requires ciel::is_assignable_v<T1&, U1> && ciel::is_assignable_v<T2&, U2> {
			first = ciel::forward<U1>(other.first);
			second = ciel::forward<U2>(other.second);
			return *this;
		}

		template<class U1, class U2>
		constexpr const pair& operator=(pair<U1, U2>&& other) const requires ciel::is_assignable_v<const T1&, U1> && ciel::is_assignable_v<const T2&, U2> {
			first = ciel::forward<U1>(other.first);
			second = ciel::forward<U2>(other.second);
			return *this;
		}

//		template<pair-like P>
//		constexpr pair& operator=(P&& u);

//		template<pair-like P>
//		constexpr const pair& operator=(P&& u) const;


		constexpr void swap(pair& other) noexcept(ciel::is_nothrow_swappable_v<T1> && ciel::is_nothrow_swappable_v<T2>) requires ciel::is_swappable_v<T1> && ciel::is_swappable_v<T2> {
			using ciel::swap;
			swap(first, other.first);
			swap(second, other.second);
		}

		constexpr void swap(const pair& other) noexcept(ciel::is_nothrow_swappable_v<const T1> && ciel::is_nothrow_swappable_v<const T2>) requires ciel::is_swappable_v<const T1> && ciel::is_swappable_v<const T2> {
			using ciel::swap;
			swap(first, other.first);
			swap(second, other.second);
		}

	};    //struct pair

	template<class T1, class T2>
	constexpr pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>> make_pair(T1&& t, T2&& u) {
		return pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>>(ciel::forward<T1>(t), ciel::forward<T2>(u));
	}

	template<class T1, class T2, class U1, class U2>
	constexpr bool operator==(const pair<T1, T2>& lhs, const pair<U1, U2>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<class T1, class T2>
	constexpr void swap(pair<T1, T2>& x, pair<T1, T2>& y) noexcept(noexcept(x.swap(y))) requires ciel::is_swappable_v<typename pair<T1, T2>::first_type> && ciel::is_swappable_v<typename pair<T1, T2>::second_type> {
		x.swap(y);
	}

	template<class T1, class T2>
	constexpr void swap(const pair<T1, T2>& x, const pair<T1, T2>& y) noexcept(noexcept(x.swap(y))) requires ciel::is_swappable_v<const typename pair<T1, T2>::first_type> && ciel::is_swappable_v<const typename pair<T1, T2>::second_type> {
		x.swap(y);
	}

	template<class T1, class T2, class U1, class U2, template<class> class TQual,
		template<class> class UQual> requires requires { typename pair<ciel::common_reference_t<TQual<T1>, UQual<U1>>, ciel::common_reference_t<TQual<T2>, UQual<U2>>>; }
	struct basic_common_reference<pair<T1, T2>, pair<U1, U2>, TQual, UQual> {
		using type = pair<ciel::common_reference_t<TQual<T1>, UQual<U1>>, ciel::common_reference_t<TQual<T2>, UQual<U2>>>;
	};

	template<class T1, class T2, class U1, class U2> requires requires { typename pair<ciel::common_type_t<T1, U1>, ciel::common_type_t<T2, U2>>; }
	struct common_type<pair<T1, T2>, pair<U1, U2>> {
		using type = pair<ciel::common_type_t<T1, U1>, ciel::common_type_t<T2, U2>>;
	};

	template<class T1, class T2>
	pair(T1, T2) -> pair<T1, T2>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_PAIR_H_