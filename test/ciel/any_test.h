#ifndef TINYSTLBYCIEL_TEST_CIEL_ANY_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_ANY_TEST_H_

#include <iostream>
#include <ciel/any.h>
#include <ciel/deque.h>
#include <ciel/vector.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void any_test() {

    ciel::any a = 1;
    CHECK(ciel::any_cast<int>(a) == 1);

    a = 3.14;
    CHECK(ciel::any_cast<double>(a) == 3.14);

    a = true;
    CHECK(ciel::any_cast<bool>(a) == true);

    // bad_any_cast
    bool flag = false;
    try {
        a = 1;
        ciel::any_cast<float>(a);
    } catch (const std::bad_any_cast& e) {
        flag = true;
    }
    CHECK(flag);

    // has_value
    a = 1;
    CHECK(a.has_value());

    // reset
    a.reset();
    CHECK(!a.has_value());

    // 指向所含数据的指针
    a = 1;
    int* i = ciel::any_cast<int>(&a);
    CHECK(*i == 1);

    // swap / 大对象
    CHECK(!ciel::any_details::is_small_object<ciel::deque<int>>::value);
    ciel::any b = ciel::deque<int>{0, 1, 2, 3, 4};
    a.swap(b);
    CHECK(ciel::any_cast<ciel::deque<int>>(a) == ciel::deque<int>({0, 1, 2, 3, 4}));
    CHECK(ciel::any_cast<int>(b) == 1);

    // emplace
    a.emplace<ciel::vector<int>>({0, 1, 2, 3, 4});
    CHECK(ciel::any_cast<ciel::vector<int>>(a) == ciel::vector<int>({0, 1, 2, 3, 4}));

    std::cout << "All any_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_ANY_TEST_H_