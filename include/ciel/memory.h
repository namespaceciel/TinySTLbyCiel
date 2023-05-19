#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_H_

//指针特征
#include <ciel/memory_impl/pointer_traits.h>

//分配器
#include <ciel/memory_impl/allocator.h>
#include <ciel/memory_impl/allocator_traits.h>

//杂项
#include <ciel/memory_impl/addressof.h>

//未初始化存储
#include <ciel/memory_impl/uninitialized_copy.h>
#include <ciel/memory_impl/uninitialized_copy_n.h>
#include <ciel/memory_impl/uninitialized_fill.h>
#include <ciel/memory_impl/uninitialized_fill_n.h>
#include <ciel/memory_impl/uninitialized_move.h>
#include <ciel/memory_impl/uninitialized_move_n.h>

#include <ciel/memory_impl/construct_at.h>
#include <ciel/memory_impl/destroy_at.h>
#include <ciel/memory_impl/destroy.h>
#include <ciel/memory_impl/destroy_n.h>

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_H_