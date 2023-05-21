#ifndef TINYSTLBYCIEL_TEST_CIEL_TYPE_TRAITS_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_TYPE_TRAITS_TEST_H_

#include <iostream>
#include <ciel/type_traits.h>

namespace type_traits_test_details {
	class Class {};
	struct Struct {};
	enum Enum {};
	union Union {};
	void FunctionVoidVoid() {}
	int FunctionIntDouble(double) { return 0; }
	void (* FunctionVoidVoidptr)() = FunctionVoidVoid;
	struct Pod {
		int a;
	};
	struct NonPod {
		NonPod() {}
	};
	struct Virtual {
		virtual ~Virtual() {}
	};
	struct NotAlign {
		char a;
		int b;
	};
	struct Abstract {
		virtual ~Abstract() = 0;
	};
	struct Final final {};
	enum struct EnumStruct { oz };
	enum class EnumClass : int {};
	class A {};
	class B : public A {};
	class C : private B {};    //B 是 C 的基类，但 private 继承使得 C 不能转为 B
	struct ClassOperatorInt {
		operator int() { return 0; }
	};
	constexpr int ConstantEvaluated() {
		if (ciel::is_constant_evaluated()) {
			return 1;
		} else {
			return 0;
		}
	}
}

void type_traits_test() {
	using namespace type_traits_test_details;

	//integral_constant

	//is_void
	static_assert(ciel::is_void_v<void>);
	static_assert(ciel::is_void_v<const volatile void>);
	static_assert(!ciel::is_void_v<int>);

	//is_null_pointer
	static_assert(ciel::is_null_pointer_v<decltype(nullptr)>);
	static_assert(!ciel::is_null_pointer_v<decltype(NULL)>);
	static_assert(ciel::is_null_pointer_v<nullptr_t>);
	static_assert(!ciel::is_null_pointer_v<void*>);
	static_assert(!ciel::is_null_pointer_v<intptr_t>);

	//is_integral
	static_assert(ciel::is_integral_v<int>);
	static_assert(ciel::is_integral_v<const volatile int>);
	static_assert(ciel::is_integral_v<bool>);
	static_assert(ciel::is_integral_v<char8_t>);
	static_assert(ciel::is_integral_v<char16_t>);
	static_assert(ciel::is_integral_v<char32_t>);
	static_assert(ciel::is_integral_v<char>);
	static_assert(ciel::is_integral_v<wchar_t>);
	static_assert(ciel::is_integral_v<signed char>);
	static_assert(ciel::is_integral_v<unsigned char>);
	static_assert(ciel::is_integral_v<long>);
	static_assert(ciel::is_integral_v<long long>);
	static_assert(ciel::is_integral_v<short>);
	static_assert(ciel::is_integral_v<unsigned int>);
	static_assert(ciel::is_integral_v<unsigned short>);
	static_assert(ciel::is_integral_v<unsigned long>);
	static_assert(ciel::is_integral_v<unsigned long long>);
	static_assert(!ciel::is_integral_v<float>);
	static_assert(!ciel::is_integral_v<double>);
	static_assert(!ciel::is_integral_v<long double>);

	//is_floating_point
	static_assert(ciel::is_floating_point_v<double>);
	static_assert(ciel::is_floating_point_v<const volatile double>);
	static_assert(ciel::is_floating_point_v<float>);
	static_assert(ciel::is_floating_point_v<long double>);
	static_assert(!ciel::is_floating_point_v<int>);

	//is_array
	static_assert(ciel::is_array_v<int[]>);
	static_assert(ciel::is_array_v<int[5]>);
	static_assert(ciel::is_array_v<int[5][5]>);
	static_assert(!ciel::is_array_v<int>);

	//is_enum
	static_assert(ciel::is_enum_v<Enum>);
	static_assert(ciel::is_enum_v<const volatile Enum>);
	static_assert(!ciel::is_enum_v<Enum*>);
	static_assert(!ciel::is_enum_v<Class>);
	static_assert(!ciel::is_enum_v<Union>);
	static_assert(!ciel::is_enum_v<Enum&>);
	static_assert(!ciel::is_enum_v<Enum&&>);

	//is_union
	static_assert(ciel::is_union_v<Union>);
	static_assert(ciel::is_union_v<const volatile Union>);
	static_assert(!ciel::is_union_v<Union*>);
	static_assert(!ciel::is_union_v<Class>);
	static_assert(!ciel::is_union_v<Enum>);
	static_assert(!ciel::is_union_v<Union&>);
	static_assert(!ciel::is_union_v<Union&&>);

	//is_class
	static_assert(ciel::is_class_v<Class>);
	static_assert(ciel::is_class_v<const Class>);
	static_assert(ciel::is_class_v<Struct>);
	static_assert(ciel::is_class_v<volatile Struct>);
	static_assert(!ciel::is_class_v<Enum>);
	static_assert(!ciel::is_class_v<Union>);

	//is_function
	static_assert(ciel::is_function_v<decltype(FunctionVoidVoid)>);
	static_assert(ciel::is_function_v<decltype(FunctionIntDouble)>);
	static_assert(!ciel::is_function_v<decltype(FunctionVoidVoidptr)>);
	static_assert(ciel::is_function_v<int(const char*, ...)>);    //printf
	static_assert(ciel::is_function_v<int(float) noexcept>);
	static_assert(!ciel::is_function_v<void>);

	//is_pointer
	static_assert(ciel::is_pointer_v<decltype(FunctionVoidVoidptr)>);
	static_assert(ciel::is_pointer_v<int*>);
	static_assert(ciel::is_pointer_v<int**>);
	static_assert(!ciel::is_pointer_v<int[]>);
	static_assert(ciel::is_pointer_v<const void*>);
	static_assert(ciel::is_pointer_v<Class*>);
	static_assert(ciel::is_pointer_v<Enum*>);
	static_assert(ciel::is_pointer_v<Union*>);
	static_assert(!ciel::is_pointer_v<decltype(nullptr)>);    //nullptr_t 不是指针类型，但是可以隐式转换为任何指针类型
	static_assert(!ciel::is_pointer_v<nullptr_t>);    //nullptr_t 不是指针类型，但是可以隐式转换为任何指针类型

	//is_lvalue_reference
	static_assert(ciel::is_lvalue_reference_v<int&>);
	static_assert(ciel::is_lvalue_reference_v<const volatile int&>);
	static_assert(!ciel::is_lvalue_reference_v<int&&>);
	static_assert(!ciel::is_lvalue_reference_v<int>);

	//is_rvalue_reference
	static_assert(ciel::is_rvalue_reference_v<int&&>);
	static_assert(ciel::is_rvalue_reference_v<const volatile int&&>);
	static_assert(!ciel::is_rvalue_reference_v<int&>);
	static_assert(!ciel::is_rvalue_reference_v<int>);

	// is_member_object_pointer
	static_assert(ciel::is_member_object_pointer_v<int (Class::*)>);
	static_assert(!ciel::is_member_object_pointer_v<int (Class::*)()>);
	static_assert(!ciel::is_member_object_pointer_v<void>);

	// is_member_function_pointer
	static_assert(ciel::is_member_function_pointer_v<int (Class::*)()>);
	static_assert(ciel::is_member_function_pointer_v<int (Class::*)(...)>);
	static_assert(ciel::is_member_function_pointer_v<int (Class::*)() const noexcept>);
	static_assert(ciel::is_member_function_pointer_v<int (Class::*)()&>);
	static_assert(ciel::is_member_function_pointer_v<int (Class::*)()&&>);
	static_assert(!ciel::is_member_function_pointer_v<int (Class::*)>);

	//is_fundamental
	static_assert(ciel::is_fundamental_v<int>);
	static_assert(!ciel::is_fundamental_v<void*>);
	static_assert(ciel::is_fundamental_v<nullptr_t>);
	static_assert(!ciel::is_fundamental_v<Struct>);

	//is_arithmetic
	static_assert(ciel::is_arithmetic_v<int>);
	static_assert(ciel::is_arithmetic_v<float>);
	static_assert(!ciel::is_arithmetic_v<void>);
	static_assert(!ciel::is_arithmetic_v<nullptr_t>);

	//is_scalar
	static_assert(ciel::is_scalar_v<int*>);
	static_assert(ciel::is_scalar_v<long double>);
	static_assert(!ciel::is_scalar_v<void>);
	static_assert(ciel::is_scalar_v<nullptr_t>);
	static_assert(!ciel::is_scalar_v<Struct>);
	static_assert(ciel::is_scalar_v<Enum>);

	//is_object
	static_assert(ciel::is_object_v<int>);
	static_assert(!ciel::is_object_v<void>);
	static_assert(ciel::is_object_v<Class>);
	static_assert(ciel::is_object_v<Enum>);
	static_assert(ciel::is_object_v<Union>);
	static_assert(ciel::is_object_v<Union*>);
	static_assert(!ciel::is_object_v<Union&>);

	//is_compound
	static_assert(ciel::is_compound_v<Class>);
	static_assert(ciel::is_compound_v<Union>);
	static_assert(ciel::is_compound_v<Enum>);
	static_assert(ciel::is_compound_v<Class&>);
	static_assert(ciel::is_compound_v<Class&&>);

	//is_reference
	static_assert(ciel::is_reference_v<int&>);
	static_assert(ciel::is_reference_v<const volatile int&>);
	static_assert(ciel::is_reference_v<int&&>);
	static_assert(ciel::is_reference_v<Union&>);
	static_assert(ciel::is_reference_v<double&>);

	//is_member_pointer
	static_assert(!ciel::is_member_pointer_v<void*>);
	static_assert(ciel::is_member_pointer_v<int (Struct::*)()>);
	static_assert(ciel::is_member_pointer_v<int (Struct::*)>);
	static_assert(ciel::is_member_pointer_v<int (Struct::* const volatile)>);

	//is_const
	static_assert(ciel::is_const_v<const volatile void>);
	static_assert(!ciel::is_const_v<const int&>);
	static_assert(!ciel::is_const_v<void() const>);

	//is_volatile
	static_assert(ciel::is_volatile_v<volatile int>);
	static_assert(ciel::is_volatile_v<const volatile int>);
	static_assert(!ciel::is_volatile_v<const int>);
	static_assert(!ciel::is_volatile_v<void()>);

	//is_trivial
	static_assert(!ciel::is_trivial_v<void>);
	static_assert(ciel::is_trivial_v<int>);
	static_assert(ciel::is_trivial_v<Pod>);
	static_assert(!ciel::is_trivial_v<NonPod>);

	//is_trivially_copyable
	static_assert(!ciel::is_trivially_copyable_v<void>);
	static_assert(ciel::is_trivially_copyable_v<int>);
	static_assert(ciel::is_trivially_copyable_v<Pod>);
	static_assert(ciel::is_trivially_copyable_v<NonPod>);

	//is_standard_layout
	static_assert(ciel::is_standard_layout_v<Pod>);
	static_assert(ciel::is_standard_layout_v<NonPod>);
	static_assert(!ciel::is_standard_layout_v<Virtual>);
	static_assert(ciel::is_standard_layout_v<void*>);
	static_assert(ciel::is_standard_layout_v<int>);
	static_assert(ciel::is_standard_layout_v<int[]>);
	static_assert(!ciel::is_standard_layout_v<void>);

	//has_unique_object_representations
	static_assert(ciel::has_unique_object_representations_v<int>);
	static_assert(!ciel::has_unique_object_representations_v<void>);
	static_assert(ciel::has_unique_object_representations_v<Pod>);
	static_assert(!ciel::has_unique_object_representations_v<NotAlign>);

	//is_empty
	static_assert(ciel::is_empty_v<Struct>);
	static_assert(!ciel::is_empty_v<Pod>);
	static_assert(!ciel::is_empty_v<Enum>);
	static_assert(!ciel::is_empty_v<Union>);
	static_assert(!ciel::is_empty_v<void>);

	//is_polymorphic
	static_assert(!ciel::is_polymorphic_v<Struct>);
	static_assert(ciel::is_polymorphic_v<Virtual>);
	static_assert(!ciel::is_polymorphic_v<Virtual*>);

	//is_abstract
	static_assert(ciel::is_abstract_v<Abstract>);
	static_assert(!ciel::is_abstract_v<Virtual>);

	//is_final
	static_assert(ciel::is_final_v<Final>);
	static_assert(!ciel::is_final_v<Virtual>);
	static_assert(!ciel::is_final_v<Class>);

	//is_aggregate
	static_assert(ciel::is_aggregate_v<int[]>);
	static_assert(!ciel::is_aggregate_v<int>);
	static_assert(ciel::is_aggregate_v<NotAlign>);
	static_assert(ciel::is_aggregate_v<Union>);
	static_assert(!ciel::is_aggregate_v<Virtual>);

	//is_signed
	static_assert(!ciel::is_signed_v<bool>);
	static_assert(ciel::is_signed_v<int>);
	static_assert(ciel::is_signed_v<double>);
	static_assert(ciel::is_signed_v<long double>);
	static_assert(ciel::is_signed_v<float>);
	static_assert(ciel::is_signed_v<short>);
	static_assert(!ciel::is_signed_v<size_t>);
	static_assert(!ciel::is_signed_v<unsigned char>);

	//is_unsigned
	static_assert(ciel::is_unsigned_v<bool>);
	static_assert(ciel::is_unsigned_v<size_t>);
	static_assert(ciel::is_unsigned_v<char8_t>);
	static_assert(ciel::is_unsigned_v<unsigned long long>);
	static_assert(ciel::is_unsigned_v<unsigned char>);

	//is_bounded_array
	static_assert(!ciel::is_bounded_array_v<int[]>);
	static_assert(ciel::is_bounded_array_v<int[5]>);
	static_assert(ciel::is_bounded_array_v<int[5][5]>);

	//is_unbounded_array
	static_assert(ciel::is_unbounded_array_v<int[]>);
	static_assert(!ciel::is_unbounded_array_v<int[1]>);
	static_assert(!ciel::is_unbounded_array_v<int[5][5]>);
	static_assert(ciel::is_unbounded_array_v<int[][5]>);

	//is_scoped_enum
	static_assert(ciel::is_scoped_enum_v<EnumClass>);
	static_assert(ciel::is_scoped_enum_v<EnumStruct>);
	static_assert(!ciel::is_scoped_enum_v<Enum>);

	//is_constructible
	//is_trivially_constructible
	//is_nothrow_constructible

	//is_default_constructible
	//is_trivially_default_constructible
	//is_nothrow_default_constructible

	//is_copy_constructible
	//is_trivially_copy_constructible
	//is_nothrow_copy_constructible

	//is_move_constructible
	//is_trivially_move_constructible
	//is_nothrow_move_constructible

	//is_assignable
	//is_trivially_assignable
	//is_nothrow_assignable

	//is_copy_assignable
	//is_trivially_copy_assignable
	//is_nothrow_copy_assignable

	//is_move_assignable
	//is_trivially_move_assignable
	//is_nothrow_move_assignable

	//is_destructible
	//is_trivially_destructible
	//is_nothrow_destructible

	//has_virtual_destructor
	static_assert(!ciel::has_virtual_destructor_v<Struct>);
	static_assert(ciel::has_virtual_destructor_v<Virtual>);
	static_assert(ciel::has_virtual_destructor_v<Abstract>);

	//is_swappable_with
	//is_swappable
	//is_nothrow_swappable_with
	//is_nothrow_swappable

	//alignment_of
	static_assert(ciel::alignment_of_v<uint64_t> == 8);
	static_assert(ciel::alignment_of_v<NotAlign> == 4);

	//rank
	static_assert(ciel::rank_v<int[1][2][3][4][5][6]> == 6);
	static_assert(ciel::rank_v<int> == 0);
	static_assert(ciel::rank_v<int[][2][3][4]> == 4);
	static_assert(ciel::rank_v<int[1][2]> == 2);

	//extent
	static_assert(ciel::extent_v<int> == 0);
	static_assert(ciel::extent_v<int[]> == 0);
	static_assert(ciel::extent_v<int[][2]> == 0);
	static_assert(ciel::extent_v<int[1][2]> == 1);
	static_assert(ciel::extent_v<int, 2> == 0);
	static_assert(ciel::extent_v<int[][4][6], 1> == 4);
	static_assert(ciel::extent_v<int[][4][6], 2> == 6);

	//is_same
	static_assert(!ciel::is_same_v<char, unsigned char>);
	static_assert(!ciel::is_same_v<char, signed char>);

	//is_base_of
	static_assert(ciel::is_base_of_v<A, C>);
	static_assert(ciel::is_base_of_v<A, B>);
	static_assert(ciel::is_base_of_v<B, C>);

	//is_convertible
	static_assert(ciel::is_convertible_v<float, double>);
	static_assert(ciel::is_convertible_v<int, size_t>);
	static_assert(!ciel::is_convertible_v<C, A>);    //private 继承
	static_assert(ciel::is_convertible_v<B, A>);
	static_assert(!ciel::is_convertible_v<C, B>);    //private 继承
	static_assert(ciel::is_convertible_v<ClassOperatorInt, int>);

	//is_nothrow_convertible
	static_assert(ciel::is_nothrow_convertible_v<float, double>);
	static_assert(ciel::is_nothrow_convertible_v<int, size_t>);
	static_assert(!ciel::is_nothrow_convertible_v<C, A>);
	static_assert(ciel::is_nothrow_convertible_v<B, A>);
	static_assert(!ciel::is_nothrow_convertible_v<C, B>);
	static_assert(!ciel::is_nothrow_convertible_v<ClassOperatorInt, int>);    //没有 noexcept

	//TODO

	//remove_cv
	static_assert(ciel::is_same_v<ciel::remove_cv_t<const volatile int>, int>);
	static_assert(ciel::is_same_v<ciel::remove_cv_t<const volatile int&>, const volatile int&>);    //这里的 cv 修饰的是指向的对象，而且引用是非 cv 限定的，下同

	//remove_const
	static_assert(ciel::is_same_v<ciel::remove_const_t<const volatile int>, volatile int>);
	static_assert(ciel::is_same_v<ciel::remove_const_t<const volatile int&>, const volatile int&>);

	//remove_volatile
	static_assert(ciel::is_same_v<ciel::remove_volatile_t<const volatile int>, const int>);
	static_assert(ciel::is_same_v<ciel::remove_volatile_t<const volatile int&>, const volatile int&>);

	//add_cv
	static_assert(ciel::is_same_v<ciel::add_cv_t<int>, const volatile int>);
	static_assert(ciel::is_same_v<ciel::add_cv_t<int&>, int&>);

	//add_const
	static_assert(ciel::is_same_v<ciel::add_const_t<int>, const int>);
	static_assert(ciel::is_same_v<ciel::add_const_t<int&>, int&>);

	//add_volatile
	static_assert(ciel::is_same_v<ciel::add_volatile_t<int>, volatile int>);
	static_assert(ciel::is_same_v<ciel::add_volatile_t<int&>, int&>);

	//remove_reference
	static_assert(ciel::is_same_v<ciel::remove_reference_t<const volatile int&>, const volatile int>);
	static_assert(ciel::is_same_v<ciel::remove_reference_t<int&&>, int>);

	//add_lvalue_reference
	static_assert(ciel::is_same_v<ciel::add_lvalue_reference_t<int>, int&>);
	static_assert(ciel::is_same_v<ciel::add_lvalue_reference_t<const int>, const int&>);
	static_assert(ciel::is_same_v<ciel::add_lvalue_reference_t<void>, void>);

	//add_rvalue_reference
	static_assert(ciel::is_same_v<ciel::add_rvalue_reference_t<int>, int&&>);
	static_assert(ciel::is_same_v<ciel::add_rvalue_reference_t<int&>, int&>);
	static_assert(ciel::is_same_v<ciel::add_rvalue_reference_t<const int>, const int&&>);
	static_assert(ciel::is_same_v<ciel::add_rvalue_reference_t<void>, void>);

	//remove_pointer
	static_assert(ciel::is_same_v<ciel::remove_pointer_t<int*>, int>);
	static_assert(ciel::is_same_v<ciel::remove_pointer_t<const char*>, const char>);
	static_assert(ciel::is_same_v<ciel::remove_pointer_t<const char* const>, const char>);
	static_assert(ciel::is_same_v<ciel::remove_pointer_t<int**>, int*>);

	//add_pointer
	static_assert(ciel::is_same_v<ciel::add_pointer_t<int>, int*>);
	static_assert(ciel::is_same_v<ciel::add_pointer_t<int**>, int***>);
	static_assert(ciel::is_same_v<ciel::add_pointer_t<const char* const>, const char* const*>);

	//make_signed
	static_assert(ciel::is_same_v<ciel::make_signed_t<size_t>, long>);
	static_assert(ciel::is_same_v<ciel::make_signed_t<unsigned char>, signed char>);
	static_assert(ciel::is_same_v<ciel::make_signed_t<unsigned int>, int>);

	//make_unsigned
	static_assert(ciel::is_same_v<ciel::make_unsigned_t<long>, size_t>);
	static_assert(ciel::is_same_v<ciel::make_unsigned_t<int>, unsigned int>);
	static_assert(ciel::is_same_v<ciel::make_unsigned_t<char>, unsigned char>);

	//remove_extent
	static_assert(ciel::is_same_v<ciel::remove_extent_t<int[]>, int>);
	static_assert(ciel::is_same_v<ciel::remove_extent_t<int[6][2]>, int[2]>);
	static_assert(ciel::is_same_v<ciel::remove_extent_t<int[][4]>, int[4]>);

	//remove_all_extents
	static_assert(ciel::is_same_v<ciel::remove_all_extents_t<int[]>, int>);
	static_assert(ciel::is_same_v<ciel::remove_all_extents_t<int[6][2]>, int>);
	static_assert(ciel::is_same_v<ciel::remove_all_extents_t<int[][4]>, int>);

	//decay
	static_assert(ciel::is_same_v<ciel::decay_t<const volatile int&>, int>);
	static_assert(ciel::is_same_v<ciel::decay_t<int&&>, int>);
	static_assert(ciel::is_same_v<ciel::decay_t<int[]>, int*>);
	static_assert(ciel::is_same_v<ciel::decay_t<decltype(FunctionVoidVoidptr)>, void (*)()>);
	static_assert(ciel::is_same_v<ciel::decay_t<decltype(FunctionVoidVoidptr)>, decltype(FunctionVoidVoidptr)>);

	//remove_cvref
	static_assert(ciel::is_same_v<ciel::remove_cvref_t<const volatile int>, int>);
	static_assert(ciel::is_same_v<ciel::remove_cvref_t<const volatile int&>, int>);

	//enable_if

	//conditional
	static_assert(ciel::is_same_v<ciel::conditional_t<true, int, size_t>, int>);
	static_assert(ciel::is_same_v<ciel::conditional_t<false, int, size_t>, size_t>);

	//common_type
	static_assert(ciel::is_same_v<ciel::common_type_t<A, B>, A>);
	static_assert(ciel::is_same_v<ciel::common_type_t<int, double>, double>);
	static_assert(ciel::is_same_v<ciel::common_type_t<int, unsigned int>, unsigned int>);

	//common_reference
	static_assert(ciel::is_same_v<ciel::common_reference_t<A>, A>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<A&>, A&>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<A&&>, A&&>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<A, B>, A>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<const A&, volatile B>, A>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<const volatile A, B>, const volatile A>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<const volatile A&, B>, A>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<const volatile A, B&&>, const volatile A>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<int&, double&>, double>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<int, double>, double>);
	static_assert(ciel::is_same_v<ciel::common_reference_t<int&, long&>, long>);

	//basic_common_reference

	//underlying_type
	static_assert(ciel::is_same_v<ciel::underlying_type_t<EnumClass>, int>);
	static_assert(ciel::is_same_v<ciel::underlying_type_t<EnumStruct>, int>);
	static_assert(ciel::is_same_v<ciel::underlying_type_t<Enum>, unsigned int>);

	//TODO

	//void_t
	static_assert(ciel::is_same_v<ciel::void_t<>, void>);
	static_assert(ciel::is_same_v<ciel::void_t<const Class&>, void>);

	//type_identity

	//conjunction
	static_assert(ciel::conjunction_v<>);
	static_assert(!ciel::conjunction_v<ciel::false_type>);
	static_assert(ciel::conjunction_v<ciel::true_type, ciel::true_type, ciel::true_type, ciel::true_type>);
	static_assert(!ciel::conjunction_v<ciel::true_type, ciel::true_type, ciel::true_type, ciel::false_type>);

	//disjunction
	static_assert(!ciel::disjunction_v<>);
	static_assert(ciel::disjunction_v<ciel::true_type>);
	static_assert(ciel::disjunction_v<ciel::true_type, ciel::true_type>);
	static_assert(ciel::disjunction_v<ciel::true_type, ciel::false_type>);

	//negation
	static_assert(ciel::negation_v<ciel::false_type>);
	static_assert(!ciel::negation_v<ciel::true_type>);

	//TODO

	//is_constant_evaluated
	static_assert(ciel::is_constant_evaluated());
	static_assert(ConstantEvaluated() == 1);

	std::cout << "All type_traits_tests finished.\n\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_TYPE_TRAITS_TEST_H_