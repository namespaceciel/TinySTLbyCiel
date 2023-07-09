#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_LIMITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_LIMITS_H_

#include <ciel/type_traits.h>

namespace ciel {

    // FIXME: MSVC 的标识符定义与另两家不同，需要去掉前后下划线

	namespace limits_details {

		template<class T, bool = ciel::is_arithmetic_v<T>>
		class numeric_limits_helper {
		protected:
			using type = T;

			static constexpr bool is_specialized = false;
			static constexpr bool is_signed = false;
			static constexpr bool is_integer = false;
			static constexpr int digits = 0;

			static constexpr type min() noexcept {
				return type();
			}
			static constexpr type max() noexcept {
				return type();
			}
		};

		template<class T>
		class numeric_limits_helper<T, true> {
		protected:
			using type = T;

			static constexpr bool is_specialized = true;
			static constexpr bool is_signed = ciel::is_signed_v<type>;
			static constexpr bool is_integer = true;
			static constexpr int digits = static_cast<int>(sizeof(type) * __CHAR_BIT__ - is_signed);

			static constexpr type min() noexcept {
				return is_signed ? type(1) << digits : type(0);
			}
			static constexpr type max() noexcept {
				// 标准库实现中用的是 type(~0)，这里 ~0 首先作为 int，所有位为 1 翻译为 -1，再被各 type 初始化接收变为各自的 -1 表达
				return is_signed ? type(-1) ^ min() : type(-1);
			}
		};

		template<>
		class numeric_limits_helper<bool, true> {
		protected:
			using type = bool;

			static constexpr bool is_specialized = true;
			static constexpr bool is_signed = false;
			static constexpr bool is_integer = true;
			static constexpr int digits = 1;

			static constexpr type min() noexcept {
				return false;
			}
			static constexpr type max() noexcept {
				return true;
			}
		};

		template<>
		class numeric_limits_helper<float, true> {
		protected:
			using type = float;

			static constexpr bool is_specialized = true;
			static constexpr bool is_signed = true;
			static constexpr bool is_integer = false;
			static constexpr int digits = __FLT_MANT_DIG__;

			static constexpr type min() noexcept {
				return __FLT_MIN__;
			}
			static constexpr type max() noexcept {
				return __FLT_MAX__;
			}
		};

		template<>
		class numeric_limits_helper<double, true> {
		protected:
			using type = double;

			static constexpr bool is_specialized = true;
			static constexpr bool is_signed = true;
			static constexpr bool is_integer = false;
			static constexpr int digits = __DBL_MANT_DIG__;

			static constexpr type min() noexcept {
				return __DBL_MIN__;
			}
			static constexpr type max() noexcept {
				return __DBL_MAX__;
			}
		};

		template<>
		class numeric_limits_helper<long double, true> {
		protected:
			using type = long double;

			static constexpr bool is_specialized = true;
			static constexpr bool is_signed = true;
			static constexpr bool is_integer = false;
			static constexpr int digits = __LDBL_MANT_DIG__;

			static constexpr type min() noexcept {
				return __LDBL_MIN__;
			}
			static constexpr type max() noexcept {
				return __LDBL_MAX__;
			}
		};
	}

	template<class T>
	class numeric_limits : private limits_details::numeric_limits_helper<ciel::remove_cv_t<T>> {
	private:
		using base = limits_details::numeric_limits_helper<ciel::remove_cv_t<T>>;
		using type = typename base::type;

	public:
		static constexpr bool is_specialized = base::is_specialized;
		static constexpr bool is_signed = base::is_signed;
		static constexpr bool is_integer = base::is_integer;
		static constexpr int digits = base::digits;

		static constexpr type min() noexcept {
			return base::min();
		}
		static constexpr type max() noexcept {
			return base::max();
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_LIMITS_H_