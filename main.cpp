#include <iostream>
#include <ciel/type_traits.h>

struct A {};

struct B : A {};

struct C : B {};


int main() {
	{
		using namespace ciel;
		static_assert(is_same_v<common_reference_t<A>, A>);
		static_assert(is_same_v<common_reference_t<A&>, A&>);
		static_assert(is_same_v<common_reference_t<A&&>, A&&>);

		static_assert(is_same_v<common_reference_t<A, B, C>, A>);
		static_assert(is_same_v<common_reference_t<B, C>, B>);
		static_assert(is_same_v<common_reference_t<const A&, volatile C>, A>);
		static_assert(is_same_v<common_reference_t<const volatile A, B>, const volatile A>);
		static_assert(is_same_v<common_reference_t<const volatile A&, B>, A>);
		static_assert(is_same_v<common_reference_t<const volatile A, B&&>, const volatile A>);
		static_assert(is_same_v<common_reference_t<int&, double&>, double>);
		static_assert(is_same_v<common_reference_t<int, double>, double>);
		static_assert(is_same_v<common_reference_t<int&, long&>, long>);
	}
}