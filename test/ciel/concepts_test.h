#ifndef TINYSTLBYCIEL_TEST_CIEL_CONCEPTS_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_CONCEPTS_TEST_H_

#include <iostream>
#include <ciel/array.h>
#include <ciel/concepts.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace concepts_details{
	struct IntLike{
		int a;
	};
}

void concepts_test() {
	using namespace concepts_details;

	{
		ciel::array arr1{0, 1, 2, 3, 4};
		ciel::array arr2{4, 3, 2, 1, 0};

		ciel::ranges::swap(arr1, arr2);
		for (size_t i = 0; i < arr2.size(); ++i) {
			CHECK(arr2[i] == i);
		}
	}
	{
		int arr3[]{0,1,2,3,4};
		int arr4[]{4,3,2,1,0};

		ciel::ranges::swap(arr3,arr4);
		for (size_t i = 0; i < 5; ++i) {
			CHECK(arr4[i] == i);
		}
	}


	std::cout << "All concepts_tests finished.\n\n";
};

#endif //TINYSTLBYCIEL_TEST_CIEL_CONCEPTS_TEST_H_