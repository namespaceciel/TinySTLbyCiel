#ifndef TINYSTLBYCIEL_TEST_CIEL_CONCEPTS_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_CONCEPTS_TEST_H_

#include <iostream>
#include <ciel/array.h>
#include <ciel/concepts.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace concepts_details {
	struct IntLike {
		int v;
	};
	void swap(IntLike& lhs, int& rhs) {
		std::swap(lhs.v, rhs);
	}
	void swap(int& lhs, IntLike& rhs) {
		std::swap(lhs, rhs.v);
	}
	struct HasSwap {
		void swap(HasSwap&) {}
	};
	void swap(HasSwap&, HasSwap&) { std::cout << "HasSwap 自己定义的 swap 被调用（应打印两次）\n"; }
}

void concepts_test() {
	using namespace concepts_details;

	//ranges::swap
	{
		ciel::array arr1{0, 1, 2, 3, 4};
		ciel::array arr2{4, 3, 2, 1, 0};

		ciel::ranges::swap(arr1, arr2);
		for (size_t i = 0; i < arr2.size(); ++i) {
			CHECK(arr2[i] == i);
		}
	}
	{
		int arr3[]{0, 1, 2, 3, 4};
		int arr4[]{4, 3, 2, 1, 0};
		IntLike arr5[]{4, 3, 2, 1, 0};

		ciel::ranges::swap(arr3, arr4);
		for (size_t i = 0; i < 5; ++i) {
			CHECK(arr4[i] == i);
		}

		ciel::ranges::swap(arr4, arr5);
		for (size_t i = 0; i < 5; ++i) {
			CHECK(arr5[i].v == i);
		}
	}
	{
		HasSwap a, b;
		ciel::ranges::swap(a, b);

		using ciel::swap;
		swap(a, b);
	}

	std::cout << "All concepts_tests finished.\n\n";
};

#endif //TINYSTLBYCIEL_TEST_CIEL_CONCEPTS_TEST_H_