#ifndef TINYSTLBYCIEL_TEST_CIEL_TUPLE_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_TUPLE_TEST_H_

#include <iostream>
#include <ciel/pair_test.h>
#include <ciel/vector_test.h>
#include <ciel/tuple.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void tuple_test() {

    {
        using namespace pair_test_details;
        using namespace vector_test_details;

        const ciel::tuple<empty_struct, empty_struct> t1;
        const ciel::tuple<empty_struct, empty_struct> t2;
        t1 = t2;
        t1 = ciel::move(t2);

        const ciel::tuple<mutable_struct, mutable_struct> t3;
        const ciel::tuple<mutable_struct, mutable_struct> t4;
        t3 = t4;
        t3 = ciel::move(t4);

        ciel::tuple<ConstructAndAssignCounter, ConstructAndAssignCounter> t5;
        ciel::tuple<ConstructAndAssignCounter, ConstructAndAssignCounter> t6;
        t5 = ciel::move(t6);
        CHECK(ConstructAndAssignCounter::move == 2);

        t5.swap(t6);
        CHECK(ConstructAndAssignCounter::move == 8);

        ConstructAndAssignCounter::move = 0;
    }

    ciel::tuple t0;
    ciel::tuple<int, size_t, char> t1;
    ciel::tuple<int, size_t, char> t2(-1, 10, 'c');
    ciel::tuple t3(-1, 10UL, 'c');
    CHECK(t2 == t3);

    t1.swap(t2);
    CHECK(t1 == t3);

    t2 = t1;
    CHECK(t2 == t3);

    ciel::pair p1(-5, 't');
    ciel::tuple t4(p1);
    CHECK(t4 == ciel::tuple(-5, 't'));

    t4 = ciel::pair(-10, 'a');
    CHECK(t4 == ciel::tuple(-10, 'a'));

    t4 = ciel::move(p1);
    CHECK(t4 == ciel::tuple(-5, 't'));

    std::cout << "All tuple_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_TUPLE_TEST_H_