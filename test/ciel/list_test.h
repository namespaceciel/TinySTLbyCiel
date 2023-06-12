#ifndef TINYSTLBYCIEL_TEST_CIEL_LIST_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_LIST_TEST_H_

#include <iostream>
#include <ciel/list.h>
#include <list>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void list_test() {

	ciel::list<int> l1;
	CHECK(l1.empty());
	CHECK(l1.size() == 0);
	CHECK(l1.begin() == l1.end());

	ciel::list<int> l2(10, 666);
	CHECK(l2.size() == 10);
	CHECK(*l2.begin() == 666);
	CHECK(*--l2.end() == 666);

	ciel::list<int> l3(10);
	CHECK(l3.size() == 10);
	CHECK(*l3.begin() == 0);
	CHECK(*--l3.end() == 0);

	ciel::list<int> l4({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
	CHECK(l4.size() == 10);
	CHECK(*l4.begin() == 0);
	CHECK(*--l4.end() == 9);

	auto b1 = l4.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b1++ == i);
	}

	std::initializer_list<int> ilist{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	ciel::list<int> l5(ilist.begin(), ilist.end());
	CHECK(l5.size() == 10);
	CHECK(*l5.begin() == 0);
	CHECK(*--l5.end() == 9);
	auto b2 = l5.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b2++ == i);
	}

	ciel::list<int> l6(l5);
	CHECK(l6.size() == 10);
	CHECK(*l6.begin() == 0);
	CHECK(*--l6.end() == 9);
	auto b3 = l6.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b3++ == i);
	}

	ciel::list<int> l7(ciel::move(l6));
	CHECK(l6.empty());
	CHECK(l7.size() == 10);
	CHECK(*l7.begin() == 0);
	CHECK(*--l7.end() == 9);
	auto b4 = l7.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b4++ == i);
	}

	l7.clear();
	CHECK(l1.empty());

	l7 = ciel::move(l5);
	CHECK(l5.empty());
	CHECK(l7.size() == 10);
	CHECK(*l7.begin() == 0);
	CHECK(*--l7.end() == 9);
	auto b5 = l7.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b5++ == i);
	}

	l5 = l7;
	CHECK(l5.size() == 10);
	CHECK(*l5.begin() == 0);
	CHECK(*--l5.end() == 9);
	auto b6 = l5.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b6++ == i);
	}

	l1 = ilist;
	CHECK(l1.size() == 10);
	CHECK(*l1.begin() == 0);
	CHECK(*--l1.end() == 9);
	auto b7 = l5.begin();
	for (int i = 0; i < 10; ++i) {
		CHECK(*b7++ == i);
	}

	std::cout << "All list_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_LIST_TEST_H_