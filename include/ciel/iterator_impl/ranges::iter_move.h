#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_RANGES_ITER_MOVE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_RANGES_ITER_MOVE_H_

#include <ciel/concepts.h>

namespace ciel::ranges {

	namespace iter_move_details {

		void iter_move();

		//用于情况 1：iter_move(t)，如果 t 具有类或枚举类型且该表达式作为不求值操作数良构，其中只会以实参依赖查找所找到的候选进行 iter_move 的重载决议
		template<class T>
		concept unqualified_iter_move = ciel::class_or_enum_or_union<ciel::remove_cvref_t<T>>
			&& requires { iter_move(ciel::declval<T>()); };

		//用于情况 2：move(*t)，如果 *t 良构且它是左值
		template<class T>
		concept can_move = !unqualified_iter_move<T>
			&& requires{
				*ciel::declval<T>();
				requires ciel::is_lvalue_reference_v<decltype(*ciel::declval<T>())>;
			};

		//用于情况 3：*t，如果 *t 良构且它是右值。
		template<class T>
		concept can_deref = !unqualified_iter_move<T>
			&& !can_move<T>
			&& requires{
				*ciel::declval<T>();
				requires ciel::is_rvalue_reference_v<decltype(*ciel::declval<T>())>;
			};

		struct _fn {
			//情况 1：
			template<class T>
				requires unqualified_iter_move<T>
			constexpr decltype(auto) operator()(T&& t) const noexcept(noexcept(iter_move(ciel::forward<T>(t)))) {
				return iter_move(ciel::forward<T>(t));
			}

			//情况 2：
			template<class T>
				requires can_move<T>
			constexpr auto operator()(T&& t) const noexcept(noexcept(ciel::move(*ciel::forward<T>(t)))) -> decltype(ciel::move(*ciel::forward<T>(t))) {
				return ciel::move(*ciel::forward<T>(t));
			}

			//情况 3：
			template<class T>
				requires can_deref<T>
			constexpr auto operator()(T&& t) const noexcept(noexcept(*ciel::forward<T>(t))) -> decltype(*ciel::forward<T>(t)) {
				return *ciel::forward<T>(t);
			}
		};
	}

	inline constexpr auto iter_move = iter_move_details::_fn{};

}   //namespace ciel::ranges

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_RANGES_ITER_MOVE_H_