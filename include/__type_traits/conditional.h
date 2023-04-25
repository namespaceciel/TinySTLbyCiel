#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_CONDITIONAL_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_CONDITIONAL_H_

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
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_CONDITIONAL_H_