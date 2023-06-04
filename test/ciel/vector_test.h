#ifndef TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_

#include <iostream>
#include <ciel/vector.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace vector_test_details {
	struct ConstructAndAssignCounter {
		inline static size_t copy = 0;
		inline static size_t move = 0;

		ConstructAndAssignCounter() = default;
		ConstructAndAssignCounter(const ConstructAndAssignCounter&) { ++copy; }
		ConstructAndAssignCounter(ConstructAndAssignCounter&&) noexcept { ++move; }
		ConstructAndAssignCounter& operator=(const ConstructAndAssignCounter&) {
			++copy;
			return *this;
		}
		ConstructAndAssignCounter& operator=(ConstructAndAssignCounter&&) noexcept {
			++move;
			return *this;
		}
	};
}

void vector_test() {
	using namespace vector_test_details;
	// 这里对 vector 分 1、基础操作的正确性，2、复制与移动行为的正确性，3、异常安全 三方面进行测试

	// 1、基础操作的正确性
	{
		{
			ciel::vector<int> v1;
			CHECK(v1.empty());
			CHECK(v1.size() == 0);
			CHECK(v1.capacity() == 0);
		}
		{
			ciel::vector<int> v1(10);
			CHECK(v1.size() == 10);
			for (int i : v1) {
				CHECK(i == 0);
			}
			for (auto it = v1.begin(); it != v1.end(); ++it) {
				CHECK(*it == 0);
			}
			for (auto it = v1.rbegin(); it != v1.rend(); ++it) {
				CHECK(*it == 0);
			}
			v1.clear();
			CHECK(v1.empty());

			ciel::vector v2(10, 5);
			CHECK(v2.size() == 10);
			for (int i : v2) {
				CHECK(i == 5);
			}
			for (auto it = v2.begin(); it != v2.end(); ++it) {
				CHECK(*it == 5);
			}
			for (auto it = v2.rbegin(); it != v2.rend(); ++it) {
				CHECK(*it == 5);
			}
			v1 = v2;
			for (int i : v1) {
				CHECK(i == 5);
			}
			v1.clear();
			v1 = ciel::move(v2);
			CHECK(v2.empty());
			CHECK(v2.capacity() == 0);
			for (int i : v1) {
				CHECK(i == 5);
			}

			v1.resize(1);
			CHECK(v1 == ciel::vector({5}));

			v1.resize(10, 77);
			CHECK(v1 == ciel::vector({5, 77, 77, 77, 77, 77, 77, 77, 77, 77}));

			v1.pop_back();
			CHECK(v1 == ciel::vector({5, 77, 77, 77, 77, 77, 77, 77, 77}));

			v1.push_back(123);
			CHECK(v1 == ciel::vector({5, 77, 77, 77, 77, 77, 77, 77, 77, 123}));

			v1.shrink_to_fit();
			CHECK(v1.size() == 10);
			CHECK(v1.capacity() == 10);

			v1.reserve(98);
			CHECK(v1.size() == 10);
			CHECK(v1.capacity() == 98);

			v1.emplace_back(987);
			CHECK(v1 == ciel::vector({5, 77, 77, 77, 77, 77, 77, 77, 77, 123, 987}));

			v1.push_back(v1.size());
			CHECK(v1 == ciel::vector({5, 77, 77, 77, 77, 77, 77, 77, 77, 123, 987, 11}));

			v1.shrink_to_fit();
			CHECK(v1.capacity() == 12);
			v1.push_back(v1[0]);    // 自引用
			CHECK(v1 == ciel::vector({5, 77, 77, 77, 77, 77, 77, 77, 77, 123, 987, 11, 5}));
		}
		{
			ciel::vector v1{654};
			CHECK(v1.capacity() == 1);

			v1.push_back(v1[0]);    // 自引用
			CHECK(v1 == ciel::vector({654, 654}));
		}
		{
			ciel::vector v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
			for (size_t i = 0; i < v1.size(); ++i) {
				CHECK(v1[i] == i);
				CHECK(v1.at(i) == i);
			}

			ciel::vector v2(v1.begin(), v1.end() - 2);
			CHECK(v2.size() == 8);
			for (size_t i = 0; i < v2.size(); ++i) {
				CHECK(v2[i] == i);
			}
			CHECK(v2.front() == 0);
			CHECK(v2.back() == 7);

			ciel::vector v3(ciel::move(v1));
			for (size_t i = 0; i < v3.size(); ++i) {
				CHECK(v3[i] == i);
			}
		}
		{
			ciel::vector v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

			v1.erase(v1.begin());
			// 1, 2, 3, 4, 5, 6, 7, 8, 9
			CHECK(v1 == ciel::vector({1, 2, 3, 4, 5, 6, 7, 8, 9}));

			v1.erase(v1.begin() + 4, v1.begin() + 7);
			// 1, 2, 3, 4, 8, 9
			CHECK(v1 == ciel::vector({1, 2, 3, 4, 8, 9}));

			v1.emplace(v1.begin(), 8);
			// 8, 1, 2, 3, 4, 8, 9
			CHECK(v1 == ciel::vector({8, 1, 2, 3, 4, 8, 9}));

			v1.insert(v1.begin() + 1, {4, 3, 2});
			// 8, 4, 3, 2, 1, 2, 3, 4, 8, 9
			CHECK(v1 == ciel::vector({8, 4, 3, 2, 1, 2, 3, 4, 8, 9}));

			ciel::vector v2{123, 543, 12};
			v1.insert(v1.end(), v2.begin(), v2.begin());    // 空区间
			// 8, 4, 3, 2, 1, 2, 3, 4, 8, 9
			CHECK(v1 == ciel::vector({8, 4, 3, 2, 1, 2, 3, 4, 8, 9}));

			v1.insert(v1.end() - 1, v2.begin() + 1, v2.end());
			// 8, 4, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9
			CHECK(v1 == ciel::vector({8, 4, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

			v1.insert(v1.begin() + 2, 3, 222);
			// 8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9
			CHECK(v1 == ciel::vector({8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

			ciel::vector<int> v3;
			v3.insert(v3.begin(), v1.begin(), v1.end());
			CHECK(v3 == ciel::vector({8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

//			v1.insert(v1.begin(), v1.begin() + 1, v1.begin() + 3);    // 不合规
		}
	}

	// 2、复制与移动行为的正确性
	{
		{
			ciel::vector<ConstructAndAssignCounter> v1(5);
			CHECK(ConstructAndAssignCounter::copy == 0);

			ciel::vector<ConstructAndAssignCounter> v2(6, ConstructAndAssignCounter{});
			CHECK(ConstructAndAssignCounter::copy == 6);

			ciel::vector<ConstructAndAssignCounter> v3 = v1;
			ciel::vector<ConstructAndAssignCounter> v4 = ciel::move(v2);
			CHECK(ConstructAndAssignCounter::copy == 11);

			ciel::vector<ConstructAndAssignCounter> v5(v1.begin(), v1.end() - 1);
			CHECK(ConstructAndAssignCounter::copy == 15);

			ciel::vector<ConstructAndAssignCounter> v6({{}, {}, {}});
			CHECK(ConstructAndAssignCounter::copy == 18);

			v6 = {{}, {}, {}, {}};
			CHECK(ConstructAndAssignCounter::copy == 22);

			v6.assign(7, {});
			CHECK(ConstructAndAssignCounter::copy == 29);

			v6.assign(v1.begin(), v1.end());
			CHECK(ConstructAndAssignCounter::copy == 34);

			v6.assign({{}, {}, {}, {}});
			CHECK(ConstructAndAssignCounter::copy == 38);

			CHECK(ConstructAndAssignCounter::move == 0);
		}

		ConstructAndAssignCounter::copy = 0;
		ConstructAndAssignCounter::move = 0;

		{
			ciel::vector<ConstructAndAssignCounter> v1;

			for (size_t i = 0; i < 100; ++i) {
				v1.emplace_back();
			}
			CHECK(ConstructAndAssignCounter::copy == 0);

			for (size_t i = 0; i < 100; ++i) {
				v1.push_back({});
			}
			CHECK(ConstructAndAssignCounter::copy == 0);

			ConstructAndAssignCounter tmp;

			for (size_t i = 0; i < 100; ++i) {
				v1.push_back(ciel::move(tmp));
			}
			CHECK(ConstructAndAssignCounter::copy == 0);

			for (size_t i = 0; i < 100; ++i) {
				v1.push_back(tmp);
			}
			CHECK(ConstructAndAssignCounter::copy == 100);

//			std::cout << ConstructAndAssignCounter::move << '\n';	// ciel::vector 为 711，std::vector 为 711

			ConstructAndAssignCounter::move = 0;

			v1.shrink_to_fit();
			CHECK(ConstructAndAssignCounter::copy == 100);
			CHECK(ConstructAndAssignCounter::move == 400);
		}

		ConstructAndAssignCounter::copy = 0;
		ConstructAndAssignCounter::move = 0;

		{
			ciel::vector<ConstructAndAssignCounter> v1(10);
			v1.erase(v1.begin());    // 10 - 1
			CHECK(ConstructAndAssignCounter::copy == 0);
			CHECK(ConstructAndAssignCounter::move == 9);

			v1.erase(v1.begin() + 5, v1.begin() + 7);    // 9 - 2
			CHECK(ConstructAndAssignCounter::copy == 0);
			CHECK(ConstructAndAssignCounter::move == 11);

			v1.insert(v1.begin(), ConstructAndAssignCounter{});    // 7 + 1
			CHECK(ConstructAndAssignCounter::copy == 0);
			CHECK(ConstructAndAssignCounter::move == 19);

			ConstructAndAssignCounter tmp;
			v1.insert(v1.begin(), tmp);    // 8 + 1
			CHECK(ConstructAndAssignCounter::copy == 1);
			CHECK(ConstructAndAssignCounter::move == 27);

			v1.insert(v1.begin(), 3, {});    // 9 + 3，这里原本容量为 10，std::vector 能保证在有扩容操作时依旧一次性把元素移到正确位置，我们 ciel::vector 也可以！（
			CHECK(ConstructAndAssignCounter::copy == 4);
			CHECK(ConstructAndAssignCounter::move == 36);

			v1.insert(v1.begin(), {{}, {}});    // 容量为 20，12 + 2
			CHECK(ConstructAndAssignCounter::copy == 6);
			CHECK(ConstructAndAssignCounter::move == 48);

			v1.shrink_to_fit();    // 容量为 14
			CHECK(ConstructAndAssignCounter::copy == 6);
			CHECK(ConstructAndAssignCounter::move == 62);

			v1.insert(v1.end() - 2, v1.begin(), v1.begin() + 2);    // 14 + 2
			CHECK(ConstructAndAssignCounter::copy == 8);
			CHECK(ConstructAndAssignCounter::move == 76);
		}
	}

	// 3、异常安全

	std::cout << "All vector_tests finished.\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_