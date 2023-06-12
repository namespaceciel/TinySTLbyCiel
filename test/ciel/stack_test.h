#ifndef TINYSTLBYCIEL_TEST_CIEL_STACK_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_STACK_TEST_H_

#include <iostream>
#include <ciel/stack.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void stack_test() {
	{
		ciel::stack<int> s;
		ciel::vector<int> v;
		for (int i = 9; i >= 0; --i) {
			v.emplace_back(i);
		}
		ciel::stack s2(v);
		ciel::stack s3(ciel::move(v));
		ciel::stack s4(s2);
		ciel::stack s5(ciel::move(s3));
		std::initializer_list<int> il{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
		ciel::stack s6(il.begin(), il.end());

		CHECK(v.empty());
		CHECK(s3.empty());

		while (!s2.empty()) {
			v.emplace_back(s2.top());
			s2.pop();
		}
		CHECK(v == ciel::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}))
		v.clear();

		while (!s5.empty()) {
			v.emplace_back(s5.top());
			s5.pop();
		}
		CHECK(v == ciel::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}))
		v.clear();

		while (!s6.empty()) {
			v.emplace_back(s6.top());
			s6.pop();
		}
		CHECK(v == ciel::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}))
		v.clear();

		for (int i = 9; i >= 0; --i) {
			s2.push(i);
		}
		for (int i = 0; i < 10; ++i) {
			CHECK(s2.top() == i);
			s2.pop();
		}
	}

	std::cout << "All stack_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_STACK_TEST_H_