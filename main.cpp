#include <iostream>

int main() {
	int a = 1;
	const double& b = a;
	a = 2;
	std::cout << a << '\n' << b;
}