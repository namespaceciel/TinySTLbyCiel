#include <ciel/print.h>
#include <ciel/map.h>
#include <random>

int main() {
    std::random_device rd;
    ciel::map<size_t, size_t> map;
    std::uniform_int_distribution<size_t> dist(0, 99);
    for (size_t n = 0; n < 20000; ++n) {
        ++map[dist(rd)];
    }
    ciel::println(map);
}