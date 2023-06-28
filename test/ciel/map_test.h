#ifndef TINYSTLBYCIEL_TEST_CIEL_MAP_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_MAP_TEST_H_

#include <iostream>
#include <algorithm>
#include <random>
#include <ciel/map.h>
#include <ciel/vector.h>

#include <map>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void map_test() {

    // 构造与赋值
    {
        ciel::map<int, int> s0;

        ciel::map<int, int> s1{{0, 6},
                              {1, 7},
                              {2, 3},
                              {3, 5},
                              {4, 1},
                              {5, 0}};
        ciel::map<int, int> s2{{5, 5},
                              {3, 3},
                              {1, 7},
                              {4, 2},
                              {0, 1},
                              {2, 0}};

        ciel::vector<ciel::pair<int, int>> v1{{5, 5},
                                             {3, 3},
                                             {1, 7},
                                             {4, 2},
                                             {0, 1},
                                             {2, 0}};
        ciel::map<int, int> s3(v1.begin(), v1.end());

        ciel::map<int, int> s4{{0, 2},
                              {3, 2},
                              {1, 4},
                              {1, 2},
                              {4, 0},
                              {5, 0},
                              {3, 1},
                              {0, 5},
                              {0, 5},
                              {3, 0},
                              {2, 1},
                              {3, 4}};

        ciel::vector<ciel::pair<int, int>> v2{{5, 1},
                                             {3, 1},
                                             {4, 0},
                                             {5, 1},
                                             {3, 0},
                                             {4, 5},
                                             {0, 0},
                                             {2, 3},
                                             {1, 4},
                                             {4, 0},
                                             {2, 3}};
        ciel::map<int, int> s5(v2.begin(), v2.end());

        ciel::map s6(s5);

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
            CHECK(it->first == i++);
        }

        i = 5;
        for (auto it = s1.rbegin(); it != s1.rend(); ++it) {
            CHECK(it->first == i--);
        }

        CHECK(s1 != s2);
        CHECK(s1 != s3);
        CHECK(s1 != s4);
        CHECK(s1 != s5);
        CHECK(s1 != s6);

        ciel::map s7(ciel::move(s6));
        CHECK(s6.empty());
        CHECK(s1 != s7);

        s5 = s7;
        CHECK(s5 == s7);

        s5 = s6;
        CHECK(s5 == s6);
        CHECK(s5.empty());

        s5 = ciel::move(s7);
        CHECK(s5 != s6);
        CHECK(s7.empty());

        s5 = {{2, 1},
              {5, 3},
              {0, 4},
              {0, 1},
              {2, 3},
              {4, 5},
              {0, 2},
              {1, 3},
              {5, 4},
              {0, 1},
              {2, 3},
              {4, 5},
              {2, 1},
              {3, 4},
              {1, 2},
              {5, 0}};
        CHECK(s1 != s5);

        s7.swap(s5);
        CHECK(s5.empty());
        CHECK(s1 != s7);
    }

    // 插入和删除
    {
        ciel::map<int, int> s1;
        s1.insert({{0, 2},
                   {1, 3},
                   {4, 0},
                   {3, 4},
                   {5, 0},
                   {2, 1},
                   {3, 5},
                   {4, 0},
                   {6, 2},
                   {3, 5},
                   {2, 1},
                   {0, 2},
                   {1, 3},
                   {3, 0},
                   {6, 4},
                   {1, 2}});
        s1.emplace_hint(s1.end(), 7, 8);
        s1.emplace_hint(s1.begin(), 8, 0);
        s1.emplace(1, 2);
        s1.emplace(10, 10);
        s1.emplace_hint(s1.end(), 9, 3);
        s1.insert_or_assign(6, 3);
        s1.insert_or_assign(11, 2);
        s1[3] = 6;
        s1[12] = 1;

        ciel::map<int, int> tmp1{{0,  2},
                                {1,  3},
                                {2,  1},
                                {3,  6},
                                {4,  0},
                                {5,  0},
                                {6,  3},
                                {7,  8},
                                {8,  0},
                                {9,  3},
                                {10, 10},
                                {11, 2},
                                {12, 1}};
        CHECK(s1 == tmp1);

        s1.erase(5);
        s1.erase(0);
        s1.erase(8);

        ciel::map<int, int> tmp2{{1,  3},
                                {2,  1},
                                {3,  6},
                                {4,  0},
                                {6,  3},
                                {7,  8},
                                {9,  3},
                                {10, 10},
                                {11, 2},
                                {12, 1}};
        CHECK(s1 == tmp2);

        ciel::vector<ciel::pair<int, int>> v1{{8, 0},
                                             {5, 5}};
        s1.insert(v1.begin(), v1.end());
        s1.try_emplace(13, 1);
        s1.try_emplace(11, 11);

        ciel::map<int, int> tmp3{{1,  3},
                                {2,  1},
                                {3,  6},
                                {4,  0},
                                {5,  5},
                                {6,  3},
                                {7,  8},
                                {8,  0},
                                {9,  3},
                                {10, 10},
                                {11, 2},
                                {12, 1},
                                {13, 1}};
        CHECK(s1 == tmp3);

        s1.clear();
        CHECK(s1.empty());
    }

    // 查找
    {
        ciel::map<int, int> s1 = {{3, 1},
                                 {0, 4},
                                 {5, 7},
                                 {0, 3},
                                 {1, 2},
                                 {4, 0},
                                 {1, 0},
                                 {3, 8},
                                 {4, 6},
                                 {9, 5},
                                 {2, 9},
                                 {4, 1}};
        CHECK(s1.find(3)->second == 1);
        CHECK(s1.contains(9));
        CHECK(s1.count(0) == 1);
        CHECK(s1.lower_bound(4)->second == 0)
        CHECK(s1.upper_bound(7)->second == 5);
        CHECK(s1[5] == 7);
        CHECK(s1[2] == 9);
        CHECK(s1.begin()->second == 4);

        auto er = s1.equal_range(5);
        CHECK(ciel::distance(er.first, er.second) == 1);
    }

    std::cout << "All map_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_MAP_TEST_H_