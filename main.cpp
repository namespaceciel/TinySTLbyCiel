#include <iostream>

#include "include/type_traits.h"
#include "include/utility.h"

using namespace ciel;

struct A
{
	int m;
};

struct B
{
	int m1;
private:
	int m2;
};

struct C
{
	virtual void foo();
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << is_standard_layout<A>::value << '\n';
	std::cout << is_standard_layout<B>::value << '\n';
	std::cout << is_standard_layout<C>::value << '\n';
}