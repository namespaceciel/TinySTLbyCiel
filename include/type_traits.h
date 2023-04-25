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
//#include "__type_traits/is_enum.h"		////
//#include "__type_traits/"
//#include "__type_traits/"
#include "__type_traits/is_function.h"
#include "__type_traits/is_pointer.h"
#include "__type_traits/is_lvalue_reference.h"
#include "__type_traits/is_rvalue_reference.h"
#include "__type_traits/is_member_object_pointer.h"
#include "__type_traits/is_member_function_pointer.h"

//复合类型分类
#include "__type_traits/is_fundamental.h"
#include "__type_traits/is_arithmetic.h"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
#include "__type_traits/is_reference.h"
#include "__type_traits/is_member_pointer.h"

//类型的性质
#include "__type_traits/is_const.h"
#include "__type_traits/is_volatile.h"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"
//#include "__type_traits/"

//受支持操作

//性质查询

//类型关系
#include "__type_traits/is_same.h"

//常-易变性说明符
#include "__type_traits/remove_cv.h"

//引用
#include "__type_traits/remove_reference.h"
#include "__type_traits/add_lvalue_reference.h"
#include "__type_traits/add_rvalue_reference.h"

//指针
#include "__type_traits/remove_pointer.h"
#include "__type_traits/add_pointer.h"

//符号修饰符

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



#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_