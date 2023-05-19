#ifndef TINYSTLBYCIEL_INCLUDE_TEST_VECTOR_TEST_H_
#define TINYSTLBYCIEL_INCLUDE_TEST_VECTOR_TEST_H_

#include <iostream>
#include <ciel/vector.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void vector_test() {

	std::cout << "All vector_tests finished.\n\n";
}

#endif //TINYSTLBYCIEL_INCLUDE_TEST_VECTOR_TEST_H_