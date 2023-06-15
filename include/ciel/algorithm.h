#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_H_

// TODO 概念与约束

// 不修改序列的操作
#include <ciel/algorithm_impl/find.h>
#include <ciel/algorithm_impl/find_if.h>
#include <ciel/algorithm_impl/find_if_not.h>

// 修改序列的操作
#include <ciel/algorithm_impl/copy.h>
#include <ciel/algorithm_impl/copy_if.h>
#include <ciel/algorithm_impl/copy_n.h>
#include <ciel/algorithm_impl/copy_backward.h>
#include <ciel/algorithm_impl/move.h>
#include <ciel/algorithm_impl/move_backward.h>
#include <ciel/algorithm_impl/fill.h>
#include <ciel/algorithm_impl/fill_n.h>
#include <ciel/algorithm_impl/transform.h>
#include <ciel/algorithm_impl/generate.h>
#include <ciel/algorithm_impl/generate_n.h>
#include <ciel/algorithm_impl/remove.h>
#include <ciel/algorithm_impl/remove_if.h>

#include <ciel/utility_impl/swap.h>
#include <ciel/algorithm_impl/swap_ranges.h>
#include <ciel/algorithm_impl/iter_swap.h>

// 划分操作

// 排序操作

// （已排序范围上的）二分搜索操作

// 其他已排序范围上的操作

// （已排序范围上的）集合操作

// 堆操作
#include <ciel/algorithm_impl/is_heap.h>
#include <ciel/algorithm_impl/is_heap_until.h>
#include <ciel/algorithm_impl/make_heap.h>
#include <ciel/algorithm_impl/push_heap.h>
#include <ciel/algorithm_impl/pop_heap.h>
#include <ciel/algorithm_impl/sort_heap.h>

// 最小/最大操作

// 比较操作
#include <ciel/algorithm_impl/equal.h>

// 排列操作

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_H_