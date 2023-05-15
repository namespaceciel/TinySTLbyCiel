#include <ciel/type_traits.h>

int main() {
	static_assert(ciel::is_same_v<ciel::common_type_t<int>, int>);
}