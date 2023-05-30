#ifndef TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_

/////////////
//TODO 未完成
/////////////

#include <iostream>
#include <ciel/vector.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void vector_test() {

	std::cout << "All vector_tests finished.\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_