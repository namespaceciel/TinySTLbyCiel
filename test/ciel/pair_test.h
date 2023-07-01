#ifndef TINYSTLBYCIEL_TEST_CIEL_PAIR_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_PAIR_TEST_H_

#include <iostream>
#include <ciel/utility.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace pair_test_details {
	struct empty_struct {
		const empty_struct& operator=(const empty_struct&) const { return *this; }
		const empty_struct& operator=(empty_struct&&) const noexcept { return *this; }
	};

	struct mutable_struct {
		mutable int a{};
		const mutable_struct& operator=(const mutable_struct& other) const {
			a = other.a;
			return *this;
		}
		const mutable_struct& operator=(mutable_struct&& other) const noexcept {
			a = ciel::move(other.a);
			return *this;
		}
	};
}

void pair_test() {
	using namespace pair_test_details;

	{
		const ciel::pair<empty_struct, empty_struct> p1;
		const ciel::pair<empty_struct, empty_struct> p2;
		p1 = p2;
		p1 = ciel::move(p2);
	}
	{
		const ciel::pair<mutable_struct, mutable_struct> p3;
		const ciel::pair<mutable_struct, mutable_struct> p4;
		p3 = p4;
		p3 = ciel::move(p4);
	}
	{
		ciel::pair<size_t, double> p5(2, 1.5);
		ciel::pair<size_t, double> p6 = p5;
		CHECK(p5 == p6);
		CHECK(p5.first == p6.first);
		CHECK(p5.second == p6.second);

		ciel::pair<int, int> p7 = p6;
		CHECK(p7.first == 2);
		CHECK(p7.second == 1);

		//这里是不同模板形参的移动赋值，调用的是完美转发，不会清空 p5
		ciel::pair<int, int> p8 = ciel::move(p5);
		CHECK(p5.first == 2);
		CHECK(p5.second == 1.5);

		p5 = ciel::move(p6);
		CHECK(p5.first == 2);
		CHECK(p5.second == 1.5);
	}
	{
		ciel::pair p9(6, 2.2);
		ciel::pair p10 = ciel::make_pair(6, 2.2);
		const auto i1 = p9.first;
		volatile auto i2 = p9.second;
		ciel::pair p11 = ciel::make_pair(i1, i2);
		CHECK(p9 == p10);
		CHECK(p9 == p11);
	}
	{
		ciel::pair<int, float> p12(7, 9.1);
		ciel::pair<int, float> p13(12, 0.1);
		ciel::swap(p12, p13);
		CHECK(p12.first == 12);
		CHECK(p13.first == 7);
		//不要比较两个浮点数是否相等！
	}
	{
		static_assert(ciel::is_same_v<ciel::common_type_t<ciel::pair<const int, int>, ciel::pair<volatile int, int>>,
									  ciel::pair<int, int>>);
	}

	std::cout << "All pair_tests finished.\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_PAIR_TEST_H_