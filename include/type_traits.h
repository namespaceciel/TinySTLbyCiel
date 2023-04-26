#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_

//////////////////////////////类///////////////////////////////
//辅助类
#include "__type_traits/integral_constant.h"

//基础类型分类
#include "__type_traits/is_void.h"
#include "__type_traits/is_null_pointer.h"
#include "__type_traits/is_integral.h"
#include "__type_traits/is_floating_point.h"
#include "__type_traits/is_array.h"
#include "__type_traits/is_enum.h"/*由编译器实现*/
#include "__type_traits/is_union.h"/*由编译器实现*/
#include "__type_traits/is_class.h"/*由编译器实现*/
#include "__type_traits/is_function.h"
#include "__type_traits/is_pointer.h"
#include "__type_traits/is_lvalue_reference.h"
#include "__type_traits/is_rvalue_reference.h"
#include "__type_traits/is_member_object_pointer.h"
#include "__type_traits/is_member_function_pointer.h"

//复合类型分类
#include "__type_traits/is_fundamental.h"
#include "__type_traits/is_arithmetic.h"
#include "__type_traits/is_scalar.h"
#include "__type_traits/is_object.h"
#include "__type_traits/is_compound.h"
#include "__type_traits/is_reference.h"
#include "__type_traits/is_member_pointer.h"

//类型的性质
#include "__type_traits/is_const.h"
#include "__type_traits/is_volatile.h"
//#include "__type_traits/"
#include "__type_traits/is_trivially_copyable.h"/*由编译器实现*/
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"

//受支持操作
#include "__type_traits/is_constructible.h"/*由编译器实现*/
#include "__type_traits/is_default_constructible.h"
#include "__type_traits/is_copy_constructible.h"
#include "__type_traits/is_move_constructible.h"
#include "__type_traits/is_assignable.h"/*由编译器实现*/
#include "__type_traits/is_copy_assignable.h"
#include "__type_traits/is_move_assignable.h"
//#include "__type_traits/is_destructible.h"
#include "__type_traits/has_virtual_destructor.h"/*由编译器实现*/
//#include "__type_traits/is_swappable.h"	/////

//性质查询
#include "__type_traits/alignment_of.h"
#include "__type_traits/rank.h"
#include "__type_traits/extent.h"

//类型关系
#include "__type_traits/is_same.h"

//常-易变性说明符
#include "__type_traits/remove_cv.h"
#include "__type_traits/add_cv.h"

//引用
#include "__type_traits/remove_reference.h"
#include "__type_traits/add_lvalue_reference.h"
#include "__type_traits/add_rvalue_reference.h"

//指针
#include "__type_traits/remove_pointer.h"
#include "__type_traits/add_pointer.h"

//符号修饰符
#include "__type_traits/make_signed.h"
#include "__type_traits/make_unsigned.h"

//数组
#include "__type_traits/remove_extent.h"
#include "__type_traits/remove_all_extents.h"

//杂项变换
//#include "__type_traits/"
//#include "__type_traits/"
#include "__type_traits/decay.h"
#include "__type_traits/remove_cvref.h"
#include "__type_traits/enable_if.h"
#include "__type_traits/conditional.h"
#include "__type_traits/common_type.h"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
#include "__type_traits/void_t.h"
#include "__type_traits/type_identity.h"

//类型特征的运算

//////////////////////////////函数////////////////////////////
//成员关系

//常量求值语境

/////////////////////////////非标准////////////////////////////
#include "__type_traits/add_reference.h"
#include "__type_traits/apply_cv.h"
#include "__type_traits/type_list.h"

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_