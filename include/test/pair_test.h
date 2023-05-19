#ifndef TINYSTLBYCIEL_INCLUDE_TEST_PAIR_TEST_H_
#define TINYSTLBYCIEL_INCLUDE_TEST_PAIR_TEST_H_

#include <iostream>
#include <ciel/utility.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace pair_test_details {
	struct empty_struct {
		const empty_struct& operator=(const empty_struct&) const { return *this; }
		const empty_struct& operator=(empty_struct&&) const { return *this; }
	};

	struct mutable_struct {
		mutable int a{};
		const mutable_struct& operator=(const mutable_struct& other) const {
			a = other.a;
			return *this;
		}
		const mutable_struct& operator=(mutable_struct&& other) const {
			a = ciel::move(other.a);
			return *this;
		}
	};
}

void pair_test() {
	using namespace pair_test_details;

	{
		const ciel::pair<empty_struct, empty_struct> p1;
		const ciel::pair<empty_struct, empty_struct> p2;
		p1 = p2;
		p1 = ciel::move(p2);
	}
	{
		const ciel::pair<mutable_struct, mutable_struct> p3;
		const ciel::pair<mutable_struct, mutable_struct> p4;
		p3 = p4;
		p3 = ciel::move(p4);
	}

	std::cout << "All pair_tests finished.\n\n";
}

#endif //TINYSTLBYCIEL_INCLUDE_TEST_PAIR_TEST_H_