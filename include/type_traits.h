#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_

//////////////////////////////类///////////////////////////////
//辅助类
#include "type_traits_impl/integral_constant.h"

//基础类型分类
#include "type_traits_impl/is_void.h"
#include "type_traits_impl/is_null_pointer.h"
#include "type_traits_impl/is_integral.h"
#include "type_traits_impl/is_floating_point.h"
#include "type_traits_impl/is_array.h"
#include "type_traits_impl/is_enum.h"/*由编译器实现*/
#include "type_traits_impl/is_union.h"/*由编译器实现*/
#include "type_traits_impl/is_class.h"/*由编译器实现*/
#include "type_traits_impl/is_function.h"
#include "type_traits_impl/is_pointer.h"
#include "type_traits_impl/is_lvalue_reference.h"
#include "type_traits_impl/is_rvalue_reference.h"
#include "type_traits_impl/is_member_object_pointer.h"
#include "type_traits_impl/is_member_function_pointer.h"

//复合类型分类
#include "type_traits_impl/is_fundamental.h"
#include "type_traits_impl/is_arithmetic.h"
#include "type_traits_impl/is_scalar.h"
#include "type_traits_impl/is_object.h"
#include "type_traits_impl/is_compound.h"
#include "type_traits_impl/is_reference.h"
#include "type_traits_impl/is_member_pointer.h"

//类型的性质
#include "type_traits_impl/is_const.h"
#include "type_traits_impl/is_volatile.h"
#include "type_traits_impl/is_trivial.h"
#include "type_traits_impl/is_trivially_copyable.h"/*由编译器实现*/
#include "type_traits_impl/is_standard_layout.h"/*由编译器实现*/
#include "type_traits_impl/has_unique_object_representations.h"/*由编译器实现*/
#include "type_traits_impl/is_empty.h"/*由编译器实现*/
#include "type_traits_impl/is_polymorphic.h"
#include "type_traits_impl/is_abstract.h"/*由编译器实现*/
#include "type_traits_impl/is_final.h"/*由编译器实现*/
#include "type_traits_impl/is_aggregate.h"/*由编译器实现*/
#include "type_traits_impl/is_signed.h"
#include "type_traits_impl/is_unsigned.h"
#include "type_traits_impl/is_bounded_array.h"
#include "type_traits_impl/is_unbounded_array.h"
//#include "_type_traits/is_scoped_enum.h"

//受支持操作
#include "type_traits_impl/is_constructible.h"/*由编译器实现*/
#include "type_traits_impl/is_default_constructible.h"
#include "type_traits_impl/is_copy_constructible.h"
#include "type_traits_impl/is_move_constructible.h"
#include "type_traits_impl/is_assignable.h"/*由编译器实现*/
#include "type_traits_impl/is_copy_assignable.h"
#include "type_traits_impl/is_move_assignable.h"
#include "type_traits_impl/is_destructible.h"/*部分依靠编译器*/
#include "type_traits_impl/has_virtual_destructor.h"/*由编译器实现*/
//#include "_type_traits/is_swappable.h"

//性质查询
#include "type_traits_impl/alignment_of.h"
#include "type_traits_impl/rank.h"
#include "type_traits_impl/extent.h"

//类型关系
#include "type_traits_impl/is_same.h"
#include "type_traits_impl/is_base_of.h"
#include "type_traits_impl/is_convertible.h"
//#include "type_traits_impl/is_layout_compatible.h"
//#include "type_traits_impl/is_pointer_interconvertible_base_of.h"
//#include "type_traits_impl/is_invocable.h"

//常-易变性说明符
#include "type_traits_impl/remove_cv.h"
#include "type_traits_impl/add_cv.h"

//引用
#include "type_traits_impl/remove_reference.h"
#include "type_traits_impl/add_lvalue_reference.h"
#include "type_traits_impl/add_rvalue_reference.h"

//指针
#include "type_traits_impl/remove_pointer.h"
#include "type_traits_impl/add_pointer.h"

//符号修饰符
#include "type_traits_impl/make_signed.h"
#include "type_traits_impl/make_unsigned.h"

//数组
#include "type_traits_impl/remove_extent.h"
#include "type_traits_impl/remove_all_extents.h"

//杂项变换
#include "type_traits_impl/decay.h"
#include "type_traits_impl/remove_cvref.h"
#include "type_traits_impl/enable_if.h"
#include "type_traits_impl/conditional.h"
//#include "type_traits_impl/common_type.h"
//#include "type_traits_impl/common_reference.h"
//#include "type_traits_impl/underlying_type.h"
//#include "type_traits_impl/invoke_result.h"
#include "type_traits_impl/void_t.h"
#include "type_traits_impl/type_identity.h"

//类型特征的运算
#include "type_traits_impl/conjunction.h"
#include "type_traits_impl/disjunction.h"
#include "type_traits_impl/negation.h"

//////////////////////////////函数////////////////////////////
//成员关系
//#include "type_traits_impl/is_pointer_interconvertible_with_class.h"
//#include "type_traits_impl/is_corresponding_member.h"

//常量求值语境
//#include "type_traits_impl/is_constant_evaluated.h"

/////////////////////////////非标准////////////////////////////
#include "type_traits_impl/add_reference.h"
#include "type_traits_impl/apply_cv.h"
#include "type_traits_impl/type_list.h"

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_