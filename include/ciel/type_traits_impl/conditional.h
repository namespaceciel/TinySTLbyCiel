#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_CONDITIONAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_CONDITIONAL_H_

namespace ciel {

	template<bool B, class T, class F>
	struct conditional {
		using type = T;
	};

	template<class T, class F>
	struct conditional<false, T, F> {
		using type = F;
	};

	template<bool B, class T, class F>
	using conditional_t = typename conditional<B, T, F>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_CONDITIONAL_H_