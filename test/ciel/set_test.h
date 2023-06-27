#ifndef TINYSTLBYCIEL_TEST_CIEL_SET_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_SET_TEST_H_

#include <iostream>
#include <algorithm>
#include <random>
#include <ciel/set.h>
#include <ciel/vector.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void set_test() {

    // 构造与赋值
    {
        ciel::set<int> s0;

        ciel::set s1{0, 1, 2, 3, 4, 5};
        ciel::set s2{5, 3, 1, 4, 0, 2};

        ciel::vector v1{5, 1, 4, 0, 2, 3};
        ciel::set<int> s3(v1.begin(), v1.end());

        ciel::set s4{0, 2, 3, 2, 1, 4, 1, 2, 4, 0, 5, 0, 3, 1, 0, 5, 0, 5, 3, 0, 2, 1, 3, 4};

        ciel::vector v2{5, 1, 3, 1, 4, 0, 5, 1, 3, 0, 4, 5, 0, 0, 2, 3, 1, 4, 4, 0, 2, 3};
        ciel::set<int> s5(v2.begin(), v2.end());

        ciel::set s6(s5);

        CHECK(s0.empty());
        CHECK(s0.size() == 0);

        CHECK(s1.size() == 6);
        CHECK(s2.size() == 6);
        CHECK(s3.size() == 6);
        CHECK(s4.size() == 6);
        CHECK(s5.size() == 6);
        CHECK(s6.size() == 6);

        int i = 0;
        for (auto it = s1.begin(); it != s1.end(); ++it) {
            CHECK(*it == i++);
        }

        i = 5;
        for (auto it = s1.rbegin(); it != s1.rend(); ++it) {
            CHECK(*it == i--);
        }

        CHECK(s1 == s2);
        CHECK(s1 == s3);
        CHECK(s1 == s4);
        CHECK(s1 == s5);
        CHECK(s1 == s6);

        ciel::set<int> s7(ciel::move(s6));
        CHECK(s6.empty());
        CHECK(s1 == s7);

        s5 = s7;
        CHECK(s1 == s5);

        s5 = s6;
        CHECK(s5.empty());

        s5 = ciel::move(s7);
        CHECK(s1 == s5);
        CHECK(s7.empty());

        s5 = {2, 1, 5, 3, 0, 4, 0, 1, 2, 3, 4, 5, 0, 2, 1, 3, 5, 4, 0, 1, 2, 3, 4, 5, 2, 1, 3, 4, 1, 2, 5, 0};
        CHECK(s1 == s5);

        s7.swap(s5);
        CHECK(s5.empty());
        CHECK(s1 == s7);
    }

    // 插入和删除
    {
        ciel::set<int> s1;
        s1.insert({0, 2, 1, 3, 4, 0, 1, 2, 3, 4, 5, 0, 2, 1, 3, 5, 4, 0, 6, 2, 3, 5, 4, 0, 2, 1, 3, 5, 4, 0, 2, 1, 3, 4, 5, 5, 0, 2, 1, 3, 4, 5, 2, 3, 0, 5, 4, 1, 2});
        s1.emplace_hint(s1.end(), 7);
        s1.emplace_hint(s1.begin(), 8);
        s1.emplace(1);
        s1.emplace(10);
        s1.emplace_hint(s1.end(), 9);

        CHECK(s1 == ciel::set({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));

        s1.erase(5);
        s1.erase(0);
        s1.erase(8);

        CHECK(s1 == ciel::set({1, 2, 3, 4, 6, 7, 9, 10}));

        ciel::vector v1{8, 0, 5};
        s1.insert(v1.begin(), v1.end());
        s1.insert(12);
        s1.insert(11);

        CHECK(s1 == ciel::set({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}));

        s1.clear();
        CHECK(s1.empty());
    }

    // 查找
    {
        ciel::set s1{3, 1, 0, 4, 5, 7, 0, 3, 1, 2, 4, 0, 1, 0, 3, 8, 4, 6, 5, 3, 4, 7, 0, 3, 1, 4, 0, 7, 4, 1, 9, 5, 9, 1, 5, 1, 2, 9, 4, 1, 4};
        CHECK(*s1.find(3) == 3);
        CHECK(s1.contains(9));
        CHECK(s1.count(0) == 1);
        CHECK(*s1.lower_bound(4) == 4)
        CHECK(*s1.upper_bound(7) == 8);

        auto er = s1.equal_range(5);
        CHECK(ciel::distance(er.first, er.second) == 1);
    }

    // 使用标准库 shuffle 打乱数次，检查 ciel::set 的排序
    {
        std::random_device rd;
        std::mt19937 g(rd());

        ciel::vector<size_t> v;
        for (size_t i = 0; i < 10000; ++i) {
            v.emplace_back(i);
        }
        for (size_t loop = 0; loop < 100; ++loop) {
            std::shuffle(v.begin(), v.end(), g);

            ciel::set s(v.begin(), v.end());

            CHECK(ciel::is_sorted(s.begin(), s.end()));
        }
    }

    std::cout << "All set_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_SET_TEST_H_