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
		virtual void f() = 0;
	};
	struct Final final {};
	enum struct EnumStruct { oz };
	enum class EnumClass : int {};
}

void type_traits_test() {
	using namespace type_traits_test_details;

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
	static_assert(ciel::is_function_v<int(const char*, ...)>);
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
	static_assert(!ciel::is_pointer_v<decltype(nullptr)>);
	static_assert(!ciel::is_pointer_v<nullptr_t>);

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

	std::cout << "All type_traits_tests finished.\n\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_TYPE_TRAITS_TEST_H_