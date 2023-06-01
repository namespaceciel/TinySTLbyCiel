#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_H_

//指针特征
#include <ciel/memory_impl/pointer_traits.h>

//分配器
#include <ciel/memory_impl/allocator.h>
#include <ciel/memory_impl/allocator_traits.h>
#include <ciel/memory_impl/allocator_arg_t.h>
#include <ciel/memory_impl/uses_allocator.h>

//智能指针
#include <ciel/memory_impl/unique_ptr.h>

//辅助类
#include <ciel/memory_impl/default_delete.h>

//智能指针适配器

//常量
#include <ciel/memory_impl/allocator_arg.h>

//杂项
#include <ciel/memory_impl/to_address.h>
#include <ciel/memory_impl/addressof.h>

//未初始化存储
#include <ciel/memory_impl/uninitialized_copy.h>
#include <ciel/memory_impl/uninitialized_copy_n.h>
#include <ciel/memory_impl/uninitialized_fill.h>
#include <ciel/memory_impl/uninitialized_fill_n.h>
#include <ciel/memory_impl/uninitialized_move.h>
#include <ciel/memory_impl/uninitialized_move_n.h>
#include <ciel/memory_impl/uninitialized_default_construct.h>
#include <ciel/memory_impl/uninitialized_default_construct_n.h>
#include <ciel/memory_impl/uninitialized_value_construct.h>
#include <ciel/memory_impl/uninitialized_value_construct_n.h>
#include <ciel/memory_impl/construct_at.h>
#include <ciel/memory_impl/destroy_at.h>
#include <ciel/memory_impl/destroy.h>
#include <ciel/memory_impl/destroy_n.h>

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_H_