#include <iostream>
#include <ciel/type_traits.h>

struct S {
	char c1;
	int i;
	double d;
	char c2;
};

int main() {
	S s[2];
	std::cout << sizeof(S) << '/' << alignof(S) << '/' << sizeof(s);
}