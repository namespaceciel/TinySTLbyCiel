#include <iostream>
#include <memory>
#include "include/type_traits.h"
using namespace ciel;

struct A { };
struct B { ~B(){} };
struct C { ~C() noexcept(false) {} };

int main() {
	std::cout << std::boolalpha;
	std::cout << "is_nothrow_destructible:" << std::endl;
	std::cout << "int: " << is_nothrow_destructible<int>::value << std::endl;
	std::cout << "A: " << is_nothrow_destructible<A>::value << std::endl;
	std::cout << "B: " << is_nothrow_destructible<B>::value << std::endl;
	std::cout << "C: " << is_nothrow_destructible<C>::value << std::endl;
	return 0;
}