#include <ciel/print.h>

#include <ciel/type_traits_test.h>
#include <ciel/concepts_test.h>
#include <ciel/iterator_test.h>
#include <ciel/memory_test.h>
#include <ciel/pair_test.h>
#include <ciel/array_test.h>
#include <ciel/vector_test.h>
#include <ciel/stack_test.h>
#include <ciel/queue_test.h>
#include <ciel/list_test.h>
#include <ciel/forward_list_test.h>
#include <ciel/deque_test.h>
#include <ciel/set_test.h>
#include <ciel/map_test.h>
#include <ciel/tuple_test.h>
#include <ciel/any_test.h>

int main() {
    type_traits_test();
    concepts_test();
    iterator_test();
    memory_test();
    pair_test();
    array_test();
    vector_test();
    stack_test();
    queue_test();
    list_test();
    forward_list_test();
    deque_test();
    set_test();
    map_test();
    tuple_test();
    any_test();
}