#ifndef TINYSTLBYCIEL_TEST_CIEL_DEQUE_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_DEQUE_TEST_H_

#include <iostream>
#include <ciel/deque.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void deque_test() {

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
        }
    }

    std::cout << "All deque_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_DEQUE_TEST_H_