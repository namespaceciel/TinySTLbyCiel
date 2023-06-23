#ifndef TINYSTLBYCIEL_TEST_CIEL_DEQUE_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_DEQUE_TEST_H_

#include <iostream>
#include <ciel/deque.h>
#include <ciel/vector_test.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void deque_test() {
    using namespace vector_test_details;

    // 这里对 deque 分 1、基础操作的正确性，2、复制与移动行为的正确性，3、异常安全 三方面进行测试

    // 1、基础操作的正确性
    {
        {
            ciel::deque<int> v1;
            CHECK(v1.empty());
            CHECK(v1.size() == 0);
        }
        {
            ciel::deque<int> v1(10);
            CHECK(v1.size() == 10);
            for (int i: v1) {
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

            ciel::deque v2(10, 5);
            CHECK(v2.size() == 10);
            for (int i: v2) {
                CHECK(i == 5);
            }
            for (auto it = v2.begin(); it != v2.end(); ++it) {
                CHECK(*it == 5);
            }
            for (auto it = v2.rbegin(); it != v2.rend(); ++it) {
                CHECK(*it == 5);
            }
            v1 = v2;
            for (int i: v1) {
                CHECK(i == 5);
            }
            v1.clear();
            v1 = ciel::move(v2);
            CHECK(v2.empty());
            for (int i: v1) {
                CHECK(i == 5);
            }

            v1.resize(1);
            CHECK(v1 == ciel::deque({5}));

            v1.resize(10, 77);
            CHECK(v1 == ciel::deque({5, 77, 77, 77, 77, 77, 77, 77, 77, 77}));

            v1.pop_back();
            CHECK(v1 == ciel::deque({5, 77, 77, 77, 77, 77, 77, 77, 77}));

            v1.push_back(123);
            CHECK(v1 == ciel::deque({5, 77, 77, 77, 77, 77, 77, 77, 77, 123}));

            v1.shrink_to_fit();
            CHECK(v1.size() == 10);

            v1.emplace_back(987);
            CHECK(v1 == ciel::deque({5, 77, 77, 77, 77, 77, 77, 77, 77, 123, 987}));

            v1.push_back(v1.size());
            CHECK(v1 == ciel::deque({5, 77, 77, 77, 77, 77, 77, 77, 77, 123, 987, 11}));

            v1.shrink_to_fit();
            v1.push_back(v1[0]);    // 自引用
            CHECK(v1 == ciel::deque({5, 77, 77, 77, 77, 77, 77, 77, 77, 123, 987, 11, 5}));
        }
        {
            ciel::deque v1{654};

            v1.push_back(v1[0]);    // 自引用
            CHECK(v1 == ciel::deque({654, 654}));

            v1.resize(5000, 654);
            CHECK(v1.size() == 5000);
            for (int i: v1) {
                CHECK(i == 654);
            }
        }
        {
            ciel::deque v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
            for (size_t i = 0; i < v1.size(); ++i) {
                CHECK(v1[i] == i);
                CHECK(v1.at(i) == i);
            }

            ciel::deque v2(v1.begin(), v1.end() - 2);
            CHECK(v2.size() == 8);
            for (size_t i = 0; i < v2.size(); ++i) {
                CHECK(v2[i] == i);
            }
            CHECK(v2.front() == 0);
            CHECK(v2.back() == 7);

            ciel::deque v3(ciel::move(v1));
            for (size_t i = 0; i < v3.size(); ++i) {
                CHECK(v3[i] == i);
            }

            ciel::deque<size_t> v4;
            for (size_t i = 2000; i < 3000; ++i) {
                v4.push_back(i);
            }
            for (size_t i = 1999; i > 1000; --i) {
                v4.push_front(i);
            }
            for (size_t i = 3000; i < 4000; ++i) {
                v4.push_back(i);
            }
            for (size_t i = 1000; i > 0; --i) {
                v4.push_front(i);
            }
            v4.push_front(0ul);
            for (size_t i = 0; i < v4.size(); ++i) {
                CHECK(v4[i] == i);
            }
        }
        {
            ciel::deque v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

            v1.erase(v1.begin());
            // 1, 2, 3, 4, 5, 6, 7, 8, 9
            CHECK(v1 == ciel::deque({1, 2, 3, 4, 5, 6, 7, 8, 9}));

            v1.erase(v1.begin() + 4, v1.begin() + 7);
            // 1, 2, 3, 4, 8, 9
            CHECK(v1 == ciel::deque({1, 2, 3, 4, 8, 9}));

            v1.emplace(v1.begin(), 8);
            // 8, 1, 2, 3, 4, 8, 9
            CHECK(v1 == ciel::deque({8, 1, 2, 3, 4, 8, 9}));

            v1.insert(v1.begin() + 1, {4, 3, 2});
            // 8, 4, 3, 2, 1, 2, 3, 4, 8, 9
            CHECK(v1 == ciel::deque({8, 4, 3, 2, 1, 2, 3, 4, 8, 9}));

            ciel::deque v2{123, 543, 12};
            v1.insert(v1.end(), v2.begin(), v2.begin());    // 空区间
            // 8, 4, 3, 2, 1, 2, 3, 4, 8, 9
            CHECK(v1 == ciel::deque({8, 4, 3, 2, 1, 2, 3, 4, 8, 9}));

            v1.insert(v1.end() - 1, v2.begin() + 1, v2.end());
            // 8, 4, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9
            CHECK(v1 == ciel::deque({8, 4, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

            v1.insert(v1.begin() + 2, 3, 222);
            // 8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9
            CHECK(v1 == ciel::deque({8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

            ciel::deque<int> v3;
            v3.insert(v3.begin(), v1.begin(), v1.end());
            CHECK(v3 == ciel::deque({8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

            v3.insert(v3.end(), v1.begin(), v1.end());
            CHECK(v3 == ciel::deque({8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9, 8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9}));

            v3.insert(v3.end() - 2, 4, 9876);
            CHECK(v3 == ciel::deque({8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9, 8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 9876, 9876, 9876, 9876, 12, 9}));

            v3.insert(v3.begin() + 3, 6, 7890);
            CHECK(v3 == ciel::deque({8, 4, 222, 7890, 7890, 7890, 7890, 7890, 7890, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9, 8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 9876, 9876, 9876, 9876, 12, 9}));

//			v1.insert(v1.begin(), v1.begin() + 1, v1.begin() + 3);    // 不合规
        }
    }

    // 2、复制与移动行为的正确性
    {
        {
            ciel::deque<ConstructAndAssignCounter> v1(5);
            CHECK(ConstructAndAssignCounter::copy == 0);

            ciel::deque<ConstructAndAssignCounter> v2(6, ConstructAndAssignCounter{});
            CHECK(ConstructAndAssignCounter::copy == 6);

            ciel::deque<ConstructAndAssignCounter> v3 = v1;
            ciel::deque<ConstructAndAssignCounter> v4 = ciel::move(v2);
            CHECK(ConstructAndAssignCounter::copy == 11);

            ciel::deque<ConstructAndAssignCounter> v5(v1.begin(), v1.end() - 1);
            CHECK(ConstructAndAssignCounter::copy == 15);

            ciel::deque<ConstructAndAssignCounter> v6({{}, {}, {}});
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
            ciel::deque<ConstructAndAssignCounter> v1;

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

//			std::cout << ConstructAndAssignCounter::move << '\n';	// ciel::deque 为 200，std::deque 为 200

            ConstructAndAssignCounter::move = 0;

            v1.shrink_to_fit();
            CHECK(ConstructAndAssignCounter::copy == 100);
            CHECK(ConstructAndAssignCounter::move == 0);
        }

        ConstructAndAssignCounter::copy = 0;
        ConstructAndAssignCounter::move = 0;

        {
            ciel::deque<ConstructAndAssignCounter> v1(10);
            v1.erase(v1.begin());
            CHECK(ConstructAndAssignCounter::copy == 0);
            CHECK(ConstructAndAssignCounter::move == 0);

            v1.erase(v1.begin() + 5, v1.begin() + 7);
            CHECK(ConstructAndAssignCounter::copy == 0);
            CHECK(ConstructAndAssignCounter::move == 2);

            v1.insert(v1.begin(), ConstructAndAssignCounter{});
            CHECK(ConstructAndAssignCounter::copy == 0);
            CHECK(ConstructAndAssignCounter::move == 3);

            ConstructAndAssignCounter tmp;
            v1.insert(v1.begin(), tmp);
            CHECK(ConstructAndAssignCounter::copy == 1);
            CHECK(ConstructAndAssignCounter::move == 3);

            v1.insert(v1.begin(), 3, {});
            CHECK(ConstructAndAssignCounter::copy == 4);
            CHECK(ConstructAndAssignCounter::move == 3);

            v1.insert(v1.begin(), {{}, {}});
            CHECK(ConstructAndAssignCounter::copy == 6);
            CHECK(ConstructAndAssignCounter::move == 3);

            v1.shrink_to_fit();
            CHECK(ConstructAndAssignCounter::copy == 6);
            CHECK(ConstructAndAssignCounter::move == 3);

            v1.insert(v1.end() - 2, v1.begin(), v1.begin() + 2);
            CHECK(ConstructAndAssignCounter::copy == 8);
            CHECK(ConstructAndAssignCounter::move == 5);
        }

        ConstructAndAssignCounter::copy = 0;
        ConstructAndAssignCounter::move = 0;
    }

    // TODO: 3、异常安全

    std::cout << "All deque_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_DEQUE_TEST_H_