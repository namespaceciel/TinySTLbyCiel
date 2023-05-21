#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_RANGES_SWAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_RANGES_SWAP_H_

#include <ciel/algorithm.h>
#include <ciel/concepts_impl/class_or_enum_or_union.h>
#include <ciel/concepts_impl/move_constructible.h>
#include <ciel/concepts_impl/assignable_from.h>
#include <ciel/utility.h>
#include <algorithm>

namespace ciel::ranges {
	//ranges::swap 相比 swap 的好处在于其可以直接取代 using std::swap; swap(a, b); 具体看文档
	//https://zh.cppreference.com/w/cpp/utility/ranges/swap

	namespace swap_details {
		template<class T>
		void swap(T&, T&) = delete;

		//用于情况 1 ：
		//当 T 或 U 为对象或枚举时，代表其可能有自己实现的 swap 函数(使得自身可以跟其它类型 swap)
		//当 T 与 U 为同类型的左值引用时会动用情况 3，所以此处使其非良构
		template<class T, class U>
		concept unqualified_swappable_with = (ciel::class_or_enum_or_union<ciel::remove_cvref_t<T>> || ciel::class_or_enum_or_union<ciel::remove_cvref_t<U>>)
											&& requires(T&& t, U&& u) { swap(ciel::forward<T>(t), ciel::forward<U>(u)); };

		struct _fn;

		//用于情况 2：
		template<class T, class U, size_t Size>
		concept swappable_arrays = !unqualified_swappable_with<T(&)[Size], U(&)[Size]>
								&& ciel::extent_v<T> == ciel::extent_v<U>
								&& requires(T(& t)[Size], U(& u)[Size], const _fn& _swap){ _swap(t[0], u[0]); };

		//用于情况 3：
		template<class T>
		concept exchangeable = !unqualified_swappable_with<T&, T&>
							&& ciel::move_constructible<T>
							&& ciel::assignable_from<T&, T>;

		struct _fn {
			//情况 1：
			template<class T, class U>
				requires unqualified_swappable_with<T, U>
			constexpr void operator()(T&& t, U&& u) const {
				swap(ciel::forward<T>(t), ciel::forward<U>(u));
			}

			//情况 2（支持异质交换）：
			template<class T, class U, size_t Size>
				requires swappable_arrays<T, U, Size>
			constexpr void operator()(T(& t)[Size], U(& u)[Size]) const noexcept(noexcept((*this)(*t, *u))) {
				for (size_t i = 0; i < Size; ++i) {
					(*this)(t[i], u[i]);
				}
			}

			//情况 3：
			template<exchangeable T>
			constexpr void operator()(T& x, T& y) const noexcept(ciel::is_nothrow_move_constructible_v<T> && ciel::is_nothrow_move_assignable_v<T>) {
				y = ciel::exchange(x, ciel::move(y));
			}
		};
	}

	inline constexpr auto swap = swap_details::_fn{};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_RANGES_SWAP_H_