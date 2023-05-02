#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_INTEGRAL_CONSTANT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_INTEGRAL_CONSTANT_H_

namespace ciel {

	template<class T, T v>
	struct integral_constant {
		static constexpr T value = v;
		using value_type = T;
		using type = integral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}
		constexpr value_type operator()() const noexcept {
			return value;
		}
	};

	template<bool B>
	using bool_constant = integral_constant<bool, B>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_INTEGRAL_CONSTANT_H_