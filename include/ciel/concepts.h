#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_H_

#include <ciel/concepts_impl/boolean_testable.h>

// 核心语言概念
#include <ciel/concepts_impl/same_as.h>
#include <ciel/concepts_impl/derived_from.h>
#include <ciel/concepts_impl/convertible_to.h>
#include <ciel/concepts_impl/common_reference_with.h>
#include <ciel/concepts_impl/common_with.h>
#include <ciel/concepts_impl/integral.h>
#include <ciel/concepts_impl/signed_integral.h>
#include <ciel/concepts_impl/unsigned_integral.h>
#include <ciel/concepts_impl/floating_point.h>
#include <ciel/concepts_impl/assignable_from.h>
#include <ciel/concepts_impl/swappable.h>
#include <ciel/concepts_impl/destructible.h>
#include <ciel/concepts_impl/constructible_from.h>
#include <ciel/concepts_impl/default_initializable.h>
#include <ciel/concepts_impl/move_constructible.h>
#include <ciel/concepts_impl/copy_constructible.h>

// 比较概念
#include <ciel/concepts_impl/equality_comparable.h>
#include <ciel/concepts_impl/totally_ordered.h>

// 对象概念
#include <ciel/concepts_impl/movable.h>
#include <ciel/concepts_impl/copyable.h>
#include <ciel/concepts_impl/semiregular.h>
#include <ciel/concepts_impl/regular.h>

// 可调用概念

// 定制点对象
#include <ciel/concepts_impl/ranges_swap.h>


// 非标准
#include <ciel/concepts_impl/class_or_enum_or_union.h>

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_H_