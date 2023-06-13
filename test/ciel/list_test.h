#ifndef TINYSTLBYCIEL_TEST_CIEL_LIST_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_LIST_TEST_H_

#include <iostream>
#include <ciel/list.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void list_test() {

	// 构造与析构
	{
		ciel::list<int> l1;
		CHECK(l1.empty());
		CHECK(l1.size() == 0);
		CHECK(l1.begin() == l1.end());

		ciel::list l2(10, 666);
		CHECK(l2.size() == 10);
		CHECK(*l2.begin() == 666);
		CHECK(*--l2.end() == 666);

		ciel::list<int> l3(10);
		CHECK(l3.size() == 10);
		CHECK(*l3.begin() == 0);
		CHECK(*--l3.end() == 0);

		ciel::list l4({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
		CHECK(l4.size() == 10);
		CHECK(*l4.begin() == 0);
		CHECK(*--l4.end() == 9);

		auto b1 = l4.begin();
		for (int i = 0; i < 10; ++i) {
			CHECK(*b1++ == i);
		}

		std::initializer_list<int> ilist{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		ciel::list l5(ilist.begin(), ilist.end());
		CHECK(l5.size() == 10);
		CHECK(*l5.begin() == 0);
		CHECK(*--l5.end() == 9);
		auto b2 = l5.begin();
		for (int i = 0; i < 10; ++i) {
			CHECK(*b2++ == i);
		}

		ciel::list l6(l5);
		CHECK(l6.size() == 10);
		CHECK(*l6.begin() == 0);
		CHECK(*--l6.end() == 9);
		auto b3 = l6.begin();
		for (int i = 0; i < 10; ++i) {
			CHECK(*b3++ == i);
		}

		ciel::list l7(ciel::move(l6));
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
	}

	// assign
	{
		ciel::list l1{1, 2, 3};
		l1.assign(5, 123);
		CHECK(l1 == ciel::list({123, 123, 123, 123, 123}));

		l1.assign({432, 53, 1, 67});
		CHECK(l1 == ciel::list({432, 53, 1, 67}));

		ciel::list l2{654, 433, 21, 987, 655};
		l1.assign(l2.begin(), l2.end());
		CHECK(l1 == ciel::list({654, 433, 21, 987, 655}));
	}

	// 插入元素
	{
		ciel::list<int> l1;
		l1.push_back(3);
		l1.emplace_back(4);
		l1.emplace_front(2);
		l1.emplace_back(5);
		l1.push_front(1);
		l1.emplace_front(0);
		CHECK(l1 == ciel::list({0, 1, 2, 3, 4, 5}));

		l1.insert(l1.begin().next(), 123);
		l1.insert(l1.end(), 123);
		CHECK(l1 == ciel::list({0, 123, 1, 2, 3, 4, 5, 123}));

		l1.insert(l1.end().prev(), 3, 666);
		CHECK(l1 == ciel::list({0, 123, 1, 2, 3, 4, 5, 666, 666, 666, 123}));

		l1.insert(l1.begin(), {11, 22, 33});
		CHECK(l1 == ciel::list({11, 22, 33, 0, 123, 1, 2, 3, 4, 5, 666, 666, 666, 123}));

		ciel::list l2{98, 87, 76};
		l1.insert(l1.begin(), l2.begin(), l2.end());
		CHECK(l1 == ciel::list({98, 87, 76, 11, 22, 33, 0, 123, 1, 2, 3, 4, 5, 666, 666, 666, 123}));

		l1.emplace(l1.begin().next(), 87654);
		CHECK(l1 == ciel::list({98, 87654, 87, 76, 11, 22, 33, 0, 123, 1, 2, 3, 4, 5, 666, 666, 666, 123}));

		l1.erase(l1.begin());
		CHECK(l1 == ciel::list({87654, 87, 76, 11, 22, 33, 0, 123, 1, 2, 3, 4, 5, 666, 666, 666, 123}));

		l1.erase(l1.begin(), l1.end().prev());
		CHECK(l1 == ciel::list({123}));

		l1.resize(5);
		CHECK(l1 == ciel::list({123, 0, 0, 0, 0}));

		l1.resize(3);
		CHECK(l1 == ciel::list({123, 0, 0}));

		l1.resize(6, 123);
		CHECK(l1 == ciel::list({123, 0, 0, 123, 123, 123}));

		l1.pop_back();
		l1.pop_front();
		CHECK(l1 == ciel::list({0, 0, 123, 123}));
	}

	// swap
	{
		ciel::list l1{4, 3, 2, 1};
		ciel::list l2{6, 7, 8, 9, 6, 4, 3};

		ciel::swap(l1, l2);
		CHECK(l1 == ciel::list({6, 7, 8, 9, 6, 4, 3}));
		CHECK(l2 == ciel::list({4, 3, 2, 1}))
	}

	std::cout << "All list_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_LIST_TEST_H_