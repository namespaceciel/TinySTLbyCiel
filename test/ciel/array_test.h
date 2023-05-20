#ifndef TINYSTLBYCIEL_TEST_CIEL_ARRAY_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_ARRAY_TEST_H_

#include <iostream>
#include <ciel/array.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void array_test() {
	ciel::array<int, 5> a = {0, 1, 2, 3, 4};
	ciel::array<int, 5> b = {4, 3, 2, 1, 0};
	ciel::array<int, 5> c = b;

	for (size_t i = 0; i < a.size(); ++i) {
		CHECK(a.at(i) == i);
		CHECK(a[i] == i);
		CHECK(a.data()[i] == i);
	}

	CHECK(a.front() == 0);
	CHECK(a.back() == 4);

	CHECK(b.front() == 4);
	CHECK(b.back() == 0);

	CHECK(c.front() == 4);
	CHECK(c.back() == 0);

	{
		auto ab = a.begin();
		auto brb = b.rbegin();
		for (; ab != a.end() && brb != b.rend(); ++ab, ++brb) {
			CHECK(*ab == *brb);
		}
	}

	{
		auto ae = a.end() - 1;
		auto bre = b.rend() - 1;
		for (; ae != a.begin() && bre != b.rbegin(); --ae, --bre) {
			CHECK(*ae == *bre);
		}
	}

	for (auto bb = b.begin(), cb = c.begin(); bb != b.end() && cb != c.end(); ++bb, ++cb) {
		CHECK(*bb == *cb);
	}

	CHECK(!a.empty());
	CHECK(!b.empty());
	CHECK(!c.empty());

	CHECK(a.size() == 5);
	CHECK(b.size() == 5);
	CHECK(c.size() == 5);

	CHECK(a.max_size() == 5);
	CHECK(b.max_size() == 5);
	CHECK(c.max_size() == 5);

	CHECK(b == c);

	for (size_t i = 0; i < b.size(); ++i) {
		CHECK(b[i] == c[i]);
		CHECK(b.data()[i] == c.data()[i]);
	}

	CHECK(ciel::get<1>(a) == ciel::get<3>(b));
	CHECK(ciel::get<2>(b) == ciel::get<2>(c));

	ciel::swap(a, b);
	CHECK(a == c);

	c.fill(10);
	for (auto i : c) {
		CHECK(i == 10);
	}

	const int x = 10;
	ciel::array d{1, 2, 3, 5, x};

	std::cout << "All array_tests finished.\n\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_ARRAY_TEST_H_