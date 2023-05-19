#ifndef TINYSTLBYCIEL_INCLUDE_TEST_PAIR_TEST_H_
#define TINYSTLBYCIEL_INCLUDE_TEST_PAIR_TEST_H_

#include <iostream>
#include <ciel/utility.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace pair_test_details {
	struct empty {
		const empty& operator=(const empty&) const { return *this; }
	};
}

void pair_test() {
	using namespace pair_test_details;

	const ciel::pair<empty, empty> p1;
	const ciel::pair<empty, empty> p2;
	p1 = p2;
	p1 = ciel::move(p2);

	std::cout << "All pair_tests finished.\n\n";
}

#endif //TINYSTLBYCIEL_INCLUDE_TEST_PAIR_TEST_H_