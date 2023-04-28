#ifndef TINYSTLBYCIEL_INCLUDE_UTILITY_IMPL_PAIR_H_
#define TINYSTLBYCIEL_INCLUDE_UTILITY_IMPL_PAIR_H_

#include "../type_traits.h"

namespace ciel {

	template<class T1, class T2>
	struct pair {
		using first_type = T1;
		using second_type = T2;

		T1 first;
		T2 second;


	};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_UTILITY_IMPL_PAIR_H_