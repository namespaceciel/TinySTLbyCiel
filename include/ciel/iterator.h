#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_H_

//迭代器概念
#include <ciel/iterator_impl/indirectly_readable.h>
#include <ciel/iterator_impl/indirectly_writable.h>
#include <ciel/iterator_impl/weakly_incrementable.h>
#include <ciel/iterator_impl/incrementable.h>
#include <ciel/iterator_impl/input_or_output_iterator.h>
#include <ciel/iterator_impl/sentinel_for.h>
#include <ciel/iterator_impl/sized_sentinel_for.h>
#include <ciel/iterator_impl/input_iterator.h>
#include <ciel/iterator_impl/output_iterator.h>
#include <ciel/iterator_impl/forward_iterator.h>
#include <ciel/iterator_impl/bidirectional_iterator.h>
#include <ciel/iterator_impl/random_access_iterator.h>
#include <ciel/iterator_impl/contiguous_iterator.h>

//间接可调用概念

//常用算法要求

//算法工具

//关联类型
#include <ciel/iterator_impl/incrementable_traits.h>
#include <ciel/iterator_impl/indirectly_readable_traits.h>
#include <ciel/iterator_impl/iter_alias.h>

//原语
#include <ciel/iterator_impl/iterator_traits.h>
#include <ciel/iterator_impl/iterator_tag.h>

//适配器
#include <ciel/iterator_impl/reverse_iterator.h>

//流迭代器

//定制点对象
#include <ciel/iterator_impl/ranges::iter_move.h>

//操作
#include <ciel/iterator_impl/advance.h>
#include <ciel/iterator_impl/distance.h>
#include <ciel/iterator_impl/next.h>
#include <ciel/iterator_impl/prev.h>

//非标准
#include <ciel/iterator_impl/wrap_iter.h>

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_H_