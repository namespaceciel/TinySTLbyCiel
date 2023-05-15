#include <string>
#include <vector>
#include <type_traits>
#include <ciel/type_traits.h>

using namespace ciel;

bool GetType(const true_type&) {
	return true;
}

bool GetType(const false_type&) {
	return false;
}

int GetType(const integral_constant<size_t, (size_t)4>&) {
	return 4;
}

int GetType(const integral_constant<size_t, (size_t)8>&) {
	return 8;
}

int GetType(const integral_constant<size_t, (size_t)16>&) {
	return 16;
}

int GetType(const integral_constant<size_t, (size_t)32>&) {
	return 32;
}

#ifdef _MSC_VER
__declspec(align(32)) class ClassAlign32{ };
#else
class ClassAlign32 {} __attribute__((aligned(32)));
#endif

struct Struct {
	// Empty
};

class Class {
	// Empty
};

class Subclass : public Class {
	// Empty
};

class ClassEmpty {
	// Empty
};

class ClassNonEmpty {
public:
	int x;
};

enum Enum {
	kValue1
};

union Union {
	int x;
	short y;
};

struct FinalStruct final {
};

class FinalClass final {
};

struct PodA {
	int mX;
};

struct PodB {
	int mX;
};

bool operator==(const PodA& a1, const PodA& a2) { return (a1.mX == a2.mX); }

struct Pod1 {
	// Empty
};

struct Pod2 {
	int mX;
	Pod1 mPod1;
};

struct Pod3 {
	Pod2 mPod2;
	int mX;
	Pod1 mPod1;
};

struct NonPod1 {
	NonPod1() {}
	virtual ~NonPod1() {}
};

struct NonPod2 {
	virtual ~NonPod2() {}
	virtual void Function() {}
};

struct HasIncrementOperator { HasIncrementOperator& operator++() { return *this; }};

template<class T>
using has_increment_operator_detection = decltype(++declval<T>());

template<typename, typename = void_t<>>
struct has_increment_operator_using_void_t : false_type {};

template<typename T>
struct has_increment_operator_using_void_t<T, void_t<has_increment_operator_detection<T>>> : true_type {};

struct ConstructibleOnlyWithNonConstReference {
	ConstructibleOnlyWithNonConstReference(ConstructibleOnlyWithNonConstReference&) {}

	ConstructibleOnlyWithNonConstReference() = delete;
	ConstructibleOnlyWithNonConstReference(const ConstructibleOnlyWithNonConstReference&) = delete;
};

struct ThrowConstructibleTest {
	ThrowConstructibleTest(const int = 0) {}
	ThrowConstructibleTest(const float) {}
};

struct NoThrowAssignable {};

struct ThrowAssignableTest {
	void operator=(const NoThrowAssignable&) {}
	void operator=(const ThrowAssignableTest&) {}
};

struct NoThrowDestructible {
	~NoThrowDestructible() {}
};

struct ThrowDestructible {
	~ThrowDestructible() noexcept(false) { throw (int()); }
};

struct ThrowDestructibleNoexceptFalse {
	virtual ~ThrowDestructibleNoexceptFalse() {}
};

struct HasTrivialConstructor {
	int x;
};

struct NoTrivialConstructor {
	NoTrivialConstructor() { px = &x; }
	int x;
	int* px;
};

struct HasTrivialCopy {
	void Function() {}
	int x;
};

struct NoTrivialCopy1 {
	virtual ~NoTrivialCopy1() {}
	virtual void Function() {}
};

struct NoTrivialCopy2 {
	NoTrivialCopy1 ntv;
};

struct NonCopyable {
	NonCopyable() : mX(0) {}
	NonCopyable(int x) : mX(x) {}

	int mX;
};

struct HasTrivialAssign {
	void Function() {}
	int x;
};

struct NoTrivialAssign1 {
	virtual ~NoTrivialAssign1() {}
	virtual void Function() {}
};

struct NoTrivialAssign2 {
	NoTrivialAssign1 nta;
};

struct Polymorphic1 {
	virtual ~Polymorphic1() {}
	virtual void Function() {}
};

struct Polymorphic2 : public Polymorphic1 {
	// Empty
};

struct Polymorphic3 {
	virtual ~Polymorphic3() {}
	virtual void Function() = 0;
};

struct NonPolymorphic1 {
	void Function() {}
};

struct Abstract {
	virtual void Function() = 0;
};

struct AbstractWithDtor {
	virtual ~AbstractWithDtor() {}
	virtual void Function() = 0;
};

struct DeletedDtor {
	~DeletedDtor() = delete;
};

struct Assignable {
	void operator=(const Assignable&) {}
	void operator=(const Pod1&) {}
};

class HiddenAssign {
public:
	HiddenAssign();

private:
	HiddenAssign(const HiddenAssign& x);
	HiddenAssign& operator=(const HiddenAssign& x);
};

// This class exercises is_convertible for the case that the class has an explicit copy constructor.
struct IsConvertibleTest1 {
	IsConvertibleTest1() {}
	IsConvertibleTest1(int, int) {}
	explicit IsConvertibleTest1(const IsConvertibleTest1&) {}
	~IsConvertibleTest1() {}
};

// Helpers for enable_if tests
template<typename T>
typename enable_if<is_floating_point<T>::value, T>::type EnableIfTestFunction(T) { return 999; }

template<typename T>
typename enable_if<is_integral<T>::value, T>::type EnableIfTestFunction(T) { return 888; }

typedef char Array[32];
typedef const char ArrayConst[32];

typedef Class& Reference;
typedef const Class& ConstReference;

typedef const int ConstInt;
typedef int Int;
typedef volatile int VolatileInt;
typedef const volatile int ConstVolatileInt;
typedef int& IntReference;
typedef const int& ConstIntReference;                   // Note here that the int is const, not the reference to the int.
typedef const volatile int& ConstVolatileIntReference;  // Note here that the int is const, not the reference to the int.


typedef void FunctionVoidVoid();
typedef int FunctionIntVoid();
typedef int FunctionIntFloat(float);
typedef void (* FunctionVoidVoidPtr)();

namespace {
	const integral_constant<int*, nullptr> gIntNullptrConstant;
	static_assert(gIntNullptrConstant() == nullptr, "");
}

void TestTypeTraits() {

	// conditional
	static_assert(sizeof(conditional<true, int8_t, int16_t>::type) == sizeof(int8_t), "conditional failure");
	static_assert(sizeof(conditional<false, int8_t, int16_t>::type) == sizeof(int16_t), "conditional failure");

	// bool_constant
	static_assert(bool_constant<is_same<int, int>::value>::value == true, "bool_constant failure");
	static_assert(bool_constant<is_same<int, short>::value>::value == false, "bool_constant failure");
	static_assert(is_same<bool_constant<false>::type, integral_constant<bool, false>::type>::value, "bool_constant failure");

	// type_identity
	static_assert(sizeof(type_identity<int>::type) == sizeof(int), "type_identity failure");
	static_assert((is_same<int, type_identity<int>::type>::value == true), "type_identity failure");
	static_assert(sizeof(type_identity_t<int>) == sizeof(int), "type_identity failure");
	static_assert((is_same_v<int, type_identity_t<int>> == true), "type_identity failure");

	// is_void
	static_assert(is_void<void>::value == true, "is_void failure");
	static_assert(is_void<const void>::value == true, "is_void failure");
	static_assert(is_void<int>::value == false, "is_void failure");

	// is_null_pointer
	static_assert(is_null_pointer<decltype(nullptr)>::value == true, "is_null_pointer failure");
	static_assert(is_null_pointer<decltype(NULL)>::value == false, "is_null_pointer failure");
	static_assert(is_null_pointer<nullptr_t>::value == true, "is_null_pointer failure");
	static_assert(is_null_pointer<void*>::value == false, "is_null_pointer failure");
	static_assert(is_null_pointer<intptr_t>::value == false, "is_null_pointer failure");

	// is_integral
	static_assert(is_integral<int>::value == true, "is_integral failure");
	static_assert(is_integral<const int>::value == true, "is_integral failure");
	static_assert(is_integral<float>::value == false, "is_integral failure");
	static_assert(is_integral<bool>::value, "is_integral failure");
	static_assert(is_integral<char8_t>::value, "is_integral failure");
	static_assert(is_integral<char16_t>::value, "is_integral failure");
	static_assert(is_integral<char32_t>::value, "is_integral failure");
	static_assert(is_integral<char>::value, "is_integral failure");
	static_assert(is_integral<int>::value, "is_integral failure");
	static_assert(is_integral<long long>::value, "is_integral failure");
	static_assert(is_integral<long>::value, "is_integral failure");
	static_assert(is_integral<short>::value, "is_integral failure");
	static_assert(is_integral<signed char>::value, "is_integral failure");
	static_assert(is_integral<unsigned char>::value, "is_integral failure");
	static_assert(is_integral<unsigned int>::value, "is_integral failure");
	static_assert(is_integral<unsigned long long>::value, "is_integral failure");
	static_assert(is_integral<unsigned long>::value, "is_integral failure");
	static_assert(is_integral<unsigned short>::value, "is_integral failure");
	static_assert(is_integral<wchar_t>::value, "is_integral failure");

	// is_floating_point
	static_assert(is_floating_point<double>::value == true, "is_floating_point failure");
	static_assert(is_floating_point<const double>::value == true, "is_floating_point failure");
	static_assert(is_floating_point<int>::value == false, "is_floating_point failure");

	// is_arithmetic
	static_assert(is_arithmetic<float>::value == true, "is_arithmetic failure");
	static_assert(is_arithmetic_v<float> == true, "is_arithmetic failure");
	static_assert(is_arithmetic<Class>::value == false, "is_arithmetic failure");
	static_assert(is_arithmetic_v<Class> == false, "is_arithmetic failure");

	// is_fundamental
	static_assert(is_fundamental<void>::value == true, "is_fundamental failure");
	static_assert(is_fundamental_v<void> == true, "is_fundamental failure");
	static_assert(is_fundamental<wchar_t>::value == true, "is_fundamental failure");
	static_assert(is_fundamental_v<wchar_t> == true, "is_fundamental failure");
	static_assert(is_fundamental<Class>::value == false, "is_fundamental failure");
	static_assert(is_fundamental_v<Class> == false, "is_fundamental failure");
	static_assert(is_fundamental<nullptr_t>::value == true, "is_fundamental failure");
	static_assert(is_fundamental_v<nullptr_t> == true, "is_fundamental failure");

	// is_array
	static_assert(is_array<Array>::value == true, "is_array failure");
	static_assert(is_array<ArrayConst>::value == true, "is_array failure");
	static_assert(is_array<int[]>::value == true, "is_array failure");
	static_assert(is_array<uint32_t>::value == false, "is_array failure");
	static_assert(is_array<uint32_t*>::value == false, "is_array failure");

	//is_bounded_array
	static_assert(is_bounded_array<Array>::value == true, "is_bounded_array failure");
	static_assert(is_bounded_array<ArrayConst>::value == true, "is_bounded_array failure");
	static_assert(is_bounded_array<int>::value == false, "is_bounded_array failure");
	static_assert(is_bounded_array<int[32]>::value == true, "is_bounded_array failure");
	static_assert(is_bounded_array<int[]>::value == false, "is_bounded_array failure");
	static_assert(is_bounded_array<uint32_t>::value == false, "is_bounded_array failure");
	static_assert(is_bounded_array<uint32_t*>::value == false, "is_bounded_array failure");

	//is_unbounded_array
	static_assert(is_unbounded_array<Array>::value == false, "is_unbounded_array failure");
	static_assert(is_unbounded_array<ArrayConst>::value == false, "is_unbounded_array failure");
	static_assert(is_unbounded_array<int>::value == false, "is_unbounded_array failure");
	static_assert(is_unbounded_array<int[32]>::value == false, "is_unbounded_array failure");
	static_assert(is_unbounded_array<int[]>::value == true, "is_unbounded_array failure");
	static_assert(is_unbounded_array<uint32_t>::value == false, "is_unbounded_array failure");
	static_assert(is_unbounded_array<uint32_t*>::value == false, "is_unbounded_array failure");

	// is_reference
	static_assert(is_reference<Class&>::value == true, "is_reference failure");
	static_assert(is_reference<Class&&>::value == true, "is_reference failure");
	static_assert(is_reference<const Class&>::value == true, "is_reference failure");
	static_assert(is_reference<const Class&&>::value == true, "is_reference failure");
	static_assert(is_reference<Class>::value == false, "is_reference failure");
	static_assert(is_reference<const Class*>::value == false, "is_reference failure");

	// is_member_function_pointer
	static_assert(is_member_function_pointer<int>::value == false, "is_member_function_pointer failure");
	static_assert(is_member_function_pointer<int (Class::*)>::value == false, "is_member_function_pointer failure");
	static_assert(is_member_function_pointer<int (Class::*)()>::value == true, "is_member_function_pointer failure");
	static_assert(is_member_function_pointer<int (Class::*)(...)>::value == true, "is_member_function_pointer failure");
	static_assert(is_member_function_pointer<int (Class::*)() noexcept>::value == true, "is_member_function_pointer failure");
	static_assert(is_member_function_pointer<int (Class::*)()&>::value == true, "is_member_function_pointer failure");
	static_assert(is_member_function_pointer<int (Class::*)()&&>::value == true, "is_member_function_pointer failure");

	// is_member_object_pointer
	static_assert(is_member_object_pointer<int>::value == false, "is_member_object_pointer failure");
	static_assert(is_member_object_pointer<int (Class::*)>::value == true, "is_member_object_pointer failure");
	static_assert(is_member_object_pointer<int (Class::*)()>::value == false, "is_member_object_pointer failure");

	// is_member_pointer
	static_assert(is_member_pointer<int>::value == false, "is_member_pointer failure");
	static_assert(is_member_pointer<int (Class::*)>::value == true, "is_member_pointer failure");
	static_assert(is_member_pointer<int (Class::*)()>::value == true, "is_member_pointer failure");
	static_assert(is_member_pointer<int (Class::* const)>::value == true, "is_member_pointer failure");
	static_assert(is_member_pointer<int (Class::* volatile)>::value == true, "is_member_pointer failure");
	static_assert(is_member_pointer<int (Class::* const volatile)>::value == true, "is_member_pointer failure");

	// is_pointer
	static_assert(is_pointer<Class*>::value == true, "is_pointer failure");
	static_assert(is_pointer<const Class*>::value == true, "is_pointer failure");
	static_assert(is_pointer<Class>::value == false, "is_pointer failure");
	static_assert(is_pointer<const Class&>::value == false, "is_pointer failure");
	static_assert(is_pointer<nullptr_t>::value == false, "is_pointer failure");

	// is_enum
	static_assert(is_enum<Enum>::value == true, "is_enum failure ");
	static_assert(is_enum_v<Enum> == true, "is_enum failure ");
	static_assert(is_enum<const Enum>::value == true, "is_enum failure ");
	static_assert(is_enum_v<const Enum> == true, "is_enum failure ");
	static_assert(is_enum<Enum*>::value == false, "is_enum failure ");
	static_assert(is_enum_v<Enum*> == false, "is_enum failure ");
	static_assert(is_enum<Class>::value == false, "is_enum failure ");
	static_assert(is_enum_v<Class> == false, "is_enum failure ");
	static_assert(is_enum<Enum&>::value == false, "is_enum failure ");
	static_assert(is_enum_v<Enum&> == false, "is_enum failure ");
	static_assert(is_enum<Enum&&>::value == false, "is_enum failure ");
	static_assert(is_enum_v<Enum&&> == false, "is_enum failure ");

	// is_union
	static_assert(is_union<Union>::value == true, "is_union failure");
	static_assert(is_union_v<Union> == true, "is_union failure");
	static_assert(is_union<int>::value == false, "is_union failure");
	static_assert(is_union_v<int> == false, "is_union failure");

	// is_class
	static_assert(is_class<Class>::value == true, "is_class failure");
	static_assert(is_class<Struct>::value == true, "is_class failure");
	static_assert(is_class<Union>::value == false, "is_class failure");
	static_assert(is_class<Enum>::value == false, "is_class failure");
	static_assert(is_class<int*>::value == false, "is_class failure");

	// is_function
	static_assert(is_function<void>::value == false, "is_function failure");
	static_assert(is_function<FunctionVoidVoid>::value == true, "is_function failure");
	static_assert(is_function<FunctionVoidVoid&>::value == false, "is_function failure");
	static_assert(is_function<FunctionIntVoid>::value == true, "is_function failure");
	static_assert(is_function<FunctionIntFloat>::value == true, "is_function failure");
	static_assert(is_function<FunctionVoidVoidPtr>::value == false, "is_function failure");
	static_assert(is_function<int>::value == false, "is_function failure");
	static_assert(is_function<int[3]>::value == false, "is_function failure");
	static_assert(is_function<int[]>::value == false, "is_function failure");
	static_assert(is_function<Class>::value == false, "is_function failure");

	static_assert(is_function<int(const char*, ...)>::value == true, "is_function failure");  // This is the signature of printf.

	static_assert(is_function<int(float)>::value == true, "is_function failure");
	static_assert(is_function<int(float) const>::value == true, "is_function failure");
	static_assert(is_function<int(float) volatile>::value == true, "is_function failure");
	static_assert(is_function<int(float) const volatile>::value == true, "is_function failure");
	static_assert(is_function<int(float)&>::value == true, "is_function failure");
	static_assert(is_function<int(float)&&>::value == true, "is_function failure");
	static_assert(is_function<int(float) noexcept>::value == true, "is_function failure");
	static_assert(is_function<FunctionIntFloat&>::value == false, "is_function failure"); // reference to function, not a l-value reference qualified function
	static_assert(is_function<FunctionIntFloat&&>::value == false, "is_function failure");

	static_assert(is_function_v<void> == false, "is_function failure");
	static_assert(is_function_v<FunctionVoidVoid> == true, "is_function failure");
	static_assert(is_function_v<FunctionVoidVoid&> == false, "is_function failure");
	static_assert(is_function_v<FunctionIntVoid> == true, "is_function failure");
	static_assert(is_function_v<FunctionIntFloat> == true, "is_function failure");
	static_assert(is_function_v<FunctionVoidVoidPtr> == false, "is_function failure");
	static_assert(is_function_v<int> == false, "is_function failure");
	static_assert(is_function_v<int[3]> == false, "is_function failure");
	static_assert(is_function_v<int[]> == false, "is_function failure");
	static_assert(is_function_v<Class> == false, "is_function failure");

	static_assert(is_function_v<int(const char*, ...)> == true, "is_function failure");  // This is the signature of printf.

	// is_object
	static_assert(is_object<int>::value == true, "is_object failure");
	static_assert(is_object<Class>::value == true, "is_object failure");
	static_assert(is_object<Class*>::value == true, "is_object failure");
	static_assert(is_object<Class&>::value == false, "is_object failure");
	static_assert(is_object<Class&&>::value == false, "is_object failure");

	// is_scalar
	static_assert(is_scalar<int>::value == true, "is_scalar failure");
	static_assert(is_scalar<double>::value == true, "is_scalar failure");
	static_assert(is_scalar<Enum>::value == true, "is_scalar failure");
	static_assert(is_scalar<const Class*>::value == true, "is_scalar failure");
	static_assert(is_scalar<nullptr_t>::value == true, "is_scalar failure");

	// is_compound
	static_assert(is_compound<Class>::value == true, "is_compound failure");
	static_assert(is_compound<const Class&>::value == true, "is_compound failure");
	static_assert(is_compound<int*>::value == true, "is_compound failure");
	static_assert(is_compound<float>::value == false, "is_compound failure");
	static_assert(is_compound<bool>::value == false, "is_compound failure");

	// is_const
	static_assert(is_const<Int>::value == false, "is_const failure");
	static_assert(is_const<ConstInt>::value == true, "is_const failure");
	static_assert(is_const<VolatileInt>::value == false, "is_const failure");
	static_assert(is_const<ConstVolatileInt>::value == true, "is_const failure");
	static_assert(is_const<IntReference>::value == false, "is_const failure");
	static_assert(is_const<ConstIntReference>::value == false, "is_const failure"); // Note here that the int is const, not the reference to the int.
	static_assert(is_const<ConstVolatileIntReference>::value == false, "is_const failure"); // Note here that the int is const, not the reference to the int.
	static_assert(is_const<void() const>::value == false, "is_const failure");

	// is_volatile
	static_assert(is_volatile<Int>::value == false, "is_volatile failure");
	static_assert(is_volatile<ConstInt>::value == false, "is_volatile failure");
	static_assert(is_volatile<VolatileInt>::value == true, "is_volatile failure");
	static_assert(is_volatile<ConstVolatileInt>::value == true, "is_volatile failure");
	static_assert(is_volatile<IntReference>::value == false, "is_volatile failure");
	static_assert(is_volatile<ConstIntReference>::value == false, "is_volatile failure");
	static_assert(is_volatile<ConstVolatileIntReference>::value == false, "is_volatile failure"); // Note here that the int is volatile, not the reference to the int.
	static_assert(is_volatile<void() const>::value == false, "is_volatile failure");

	// underlying_type
	enum UnderlyingTypeTest : uint16_t { firstVal = 0, secondVal = 1 };

	constexpr bool isUnderlyingTypeCorrect = is_same_v<underlying_type_t<UnderlyingTypeTest>, uint16_t>;
	static_assert(isUnderlyingTypeCorrect, "Wrong type for underlying_type_t.");

	// is_trivial
	// is_trivially_copyable
	// is_trivially_default_constructible
	static_assert(is_trivial<Pod1>::value == true, "is_trivial failure");
	static_assert(is_trivial<NonPod1>::value == false, "is_trivial failure");

	// is_standard_layout
	static_assert(is_standard_layout<Pod1>::value == true, "is_standard_layout<Pod1> failure");
	static_assert(is_standard_layout_v<Pod1> == true, "is_standard_layout<Pod1> failure");
	static_assert(is_standard_layout<Pod2>::value == true, "is_standard_layout<Pod2> failure");
	static_assert(is_standard_layout_v<Pod2> == true, "is_standard_layout<Pod2> failure");
	static_assert(is_standard_layout<Pod3>::value == true, "is_standard_layout<Pod3> failure");
	static_assert(is_standard_layout_v<Pod3> == true, "is_standard_layout<Pod3> failure");
	static_assert(is_standard_layout<float>::value == true, "is_standard_layout<float> failure");
	static_assert(is_standard_layout_v<float> == true, "is_standard_layout<float> failure");
	static_assert(is_standard_layout<Pod1*>::value == true, "is_standard_layout<Pod1*> failure");
	static_assert(is_standard_layout_v<Pod1*> == true, "is_standard_layout<Pod1*> failure");
	static_assert(is_standard_layout<NonPod1>::value == false, "is_standard_layout<NonPod1> failure");
	static_assert(is_standard_layout_v<NonPod1> == false, "is_standard_layout<NonPod1> failure");
	static_assert(is_standard_layout<NonPod2>::value == false, "is_standard_layout<NonPod2> failure");
	static_assert(is_standard_layout_v<NonPod2> == false, "is_standard_layout<NonPod2> failure");
	static_assert(is_standard_layout<HasTrivialConstructor>::value == true, "is_standard_layout<HasTrivialConstructor> failure");
	static_assert(is_standard_layout_v<HasTrivialConstructor> == true, "is_standard_layout<HasTrivialConstructor> failure");
	static_assert(is_standard_layout<NoTrivialConstructor>::value == true,
				  "is_standard_layout<NoTrivialConstructor> failure");        // A key difference between a POD and Standard Layout is that the latter is true if there is a constructor.
	static_assert(is_standard_layout_v<NoTrivialConstructor> == true,
				  "is_standard_layout<NoTrivialConstructor> failure");        // A key difference between a POD and Standard Layout is that the latter is true if there is a constructor.

	// is_empty
	static_assert(is_empty<ClassEmpty>::value == true, "is_empty failure");
	static_assert(is_empty<ClassNonEmpty>::value == false, "is_empty failure");
	static_assert(is_empty<int>::value == false, "is_empty failure");
	static_assert(is_empty<Enum>::value == false, "is_empty failure");

	// is_polymorphic
	static_assert(is_polymorphic<Polymorphic1>::value == true, "is_polymorphic failure");
	static_assert(is_polymorphic<Polymorphic2>::value == true, "is_polymorphic failure");
	static_assert(is_polymorphic<Polymorphic3>::value == true, "is_polymorphic failure");
	static_assert(is_polymorphic<NonPolymorphic1>::value == false, "is_polymorphic failure");
	static_assert(is_polymorphic<int>::value == false, "is_polymorphic failure");
	static_assert(is_polymorphic<Polymorphic1*>::value == false, "is_polymorphic failure");

	// is_trivially_constructible
	static_assert(is_trivially_constructible<int>::value == true, "is_trivially_constructible failure");
	static_assert(is_trivially_constructible<int*>::value == true, "is_trivially_constructible failure");
	static_assert(is_trivially_constructible<HasTrivialConstructor>::value == true, "is_trivially_constructible failure");
	static_assert(is_trivially_constructible<NoTrivialConstructor>::value == false, "is_trivially_constructible failure");
	static_assert(is_trivially_constructible<int&>::value == false, "is_trivially_constructible failure");

	// is_trivially_copyable
	static_assert(is_trivially_copyable<int>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<int*>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<HasTrivialCopy>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<NoTrivialCopy1>::value == false, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<NoTrivialCopy2>::value == false, "is_trivially_copyable failure");

	// is_trivially_destructible
	static_assert(is_trivially_destructible<int>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<int*>::value == true, "is_trivially_destructible failure");

	// is_signed
	static_assert(is_signed<int>::value == true, "is_signed failure ");
	static_assert(is_signed_v<int> == true, "is_signed failure ");
	static_assert(is_signed<const int64_t>::value == true, "is_signed failure ");
	static_assert(is_signed_v<const int64_t> == true, "is_signed failure ");
	static_assert(is_signed<uint32_t>::value == false, "is_signed failure ");
	static_assert(is_signed_v<uint32_t> == false, "is_signed failure ");
	static_assert(is_signed<bool>::value == false, "is_signed failure ");
	static_assert(is_signed_v<bool> == false, "is_signed failure ");
	static_assert(is_signed<float>::value == true, "is_signed failure ");
	static_assert(is_signed_v<float> == true, "is_signed failure ");
	static_assert(is_signed<double>::value == true, "is_signed failure ");
	static_assert(is_signed_v<double> == true, "is_signed failure ");
	static_assert(is_signed<char16_t>::value == false, "is_signed failure ");
	static_assert(is_signed_v<char16_t> == false, "is_signed failure ");
	static_assert(is_signed<char32_t>::value == false, "is_signed failure ");
	static_assert(is_signed_v<char32_t> == false, "is_signed failure ");

	// is_unsigned
	static_assert(is_unsigned<unsigned int>::value == true, "is_unsigned failure ");
	static_assert(is_unsigned_v<unsigned int> == true, "is_unsigned failure ");
	static_assert(is_unsigned<const uint64_t>::value == true, "is_unsigned failure ");
	static_assert(is_unsigned_v<const uint64_t> == true, "is_unsigned failure ");
	static_assert(is_unsigned<int32_t>::value == false, "is_unsigned failure ");
	static_assert(is_unsigned_v<int32_t> == false, "is_unsigned failure ");
	static_assert(is_unsigned<bool>::value == true, "is_unsigned failure ");
	static_assert(is_unsigned_v<bool> == true, "is_unsigned failure ");
	static_assert(is_unsigned<float>::value == false, "is_unsigned failure ");
	static_assert(is_unsigned_v<float> == false, "is_unsigned failure ");
	static_assert(is_unsigned<double>::value == false, "is_unsigned failure ");
	static_assert(is_unsigned_v<double> == false, "is_unsigned failure ");
	static_assert(is_unsigned<char16_t>::value == true, "is_unsigned failure ");
	static_assert(is_unsigned_v<char16_t> == true, "is_unsigned failure ");
	static_assert(is_unsigned<char32_t>::value == true, "is_unsigned failure ");
	static_assert(is_unsigned_v<char32_t> == true, "is_unsigned failure ");

	// is_lvalue_reference
	static_assert((is_lvalue_reference<Class>::value == false), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference<Class&>::value == true), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference<Class&&>::value == false), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference<int>::value == false), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference<int&>::value == true), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference<int&&>::value == false), "is_lvalue_reference failure");

	static_assert((is_lvalue_reference_v<Class> == false), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference_v<Class&> == true), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference_v<Class&&> == false), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference_v<int> == false), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference_v<int&> == true), "is_lvalue_reference failure");
	static_assert((is_lvalue_reference_v<int&&> == false), "is_lvalue_reference failure");

	// is_rvalue_reference
	static_assert((is_rvalue_reference<Class>::value == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference<Class&>::value == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference<Class&&>::value == true), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference<int>::value == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference<int&>::value == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference<int&&>::value == true), "is_rvalue_reference failure");

	static_assert((is_rvalue_reference_v<Class> == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference_v<Class&> == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference_v<Class&&> == true), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference_v<int> == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference_v<int&> == false), "is_rvalue_reference failure");
	static_assert((is_rvalue_reference_v<int&&> == true), "is_rvalue_reference failure");

	// is_assignable
	static_assert((is_assignable<int&, int>::value == true), "is_assignable failure");
	static_assert((is_assignable<const int&, int>::value == false), "is_assignable failure");
	static_assert((is_assignable<char*, int*>::value == false), "is_assignable failure");
	static_assert((is_assignable<char*, const char*>::value == false), "is_assignable failure");
	static_assert((is_assignable<PodA, PodB*>::value == false), "is_assignable failure");
	static_assert((is_assignable<Assignable, Pod2>::value == false), "is_assignable failure");

	static_assert((is_assignable<Assignable, Assignable>::value == true), "is_assignable failure");
	static_assert((is_assignable<Assignable, Pod1>::value == true), "is_assignable failure");
	static_assert((is_assignable<PodA&, PodA>::value == true), "is_assignable failure");

	// These cannot succeed unless the implementation is conforming.
	static_assert((is_assignable<void, void>::value == false), "is_assignable failure");
	static_assert((is_assignable<int, int>::value == false), "is_assignable failure");
	static_assert((is_assignable<int, const int>::value == false), "is_assignable failure");
	static_assert((is_assignable<const int, int>::value == false), "is_assignable failure");
	static_assert((is_assignable<int, int&>::value == false), "is_assignable failure");
	static_assert((is_assignable<int64_t, int8_t>::value == false), "is_assignable failure");
	static_assert((is_assignable<bool, bool>::value == false), "is_assignable failure");
	static_assert((is_assignable<char*, char*>::value == false), "is_assignable failure");
	static_assert((is_assignable<int, float>::value == false), "is_assignable failure");
	static_assert((is_assignable<const char*, char*>::value == false), "is_assignable failure");
	static_assert((is_assignable<int[], int[]>::value == false), "is_assignable failure");

	// is_copy_assignable
	static_assert((is_copy_assignable<int&>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<char>::value == true), "is_copy_assignable failure");

	static_assert((is_copy_assignable<Assignable>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<Assignable>::value == true), "is_copy_assignable failure");

	static_assert((is_copy_assignable<char*>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<PodA>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<Assignable>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<void>::value == false), "is_copy_assignable failure");
	static_assert((is_copy_assignable<int>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<const int>::value == false), "is_copy_assignable failure");
	static_assert((is_copy_assignable<int64_t>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<bool>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<char*>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<const char*>::value == true), "is_copy_assignable failure");
	static_assert((is_copy_assignable<int[3]>::value == false), "is_copy_assignable failure");
	static_assert((is_copy_assignable<int[]>::value == false), "is_copy_assignable failure");

	// is_trivially_assignable
	static_assert((is_trivially_assignable<int&, int>::value == true), "is_trivially_assignable failure");
	static_assert((is_trivially_assignable<char*, int*>::value == false), "is_trivially_assignable failure");
	static_assert((is_trivially_assignable<char*, const char*>::value == false), "is_trivially_assignable failure");
	static_assert((is_trivially_assignable<PodA, PodB*>::value == false), "is_trivially_assignable failure");
	static_assert((is_trivially_assignable<Assignable, Assignable>::value == false), "is_trivially_assignable failure"); // False because not trivial.
	static_assert((is_trivially_assignable<Assignable, Pod1>::value == false), "is_trivially_assignable failure"); // False because not trivial.
	static_assert((is_trivially_assignable<Assignable, Pod2>::value == false), "is_trivially_assignable failure");

	// is_nothrow_assignable
	static_assert((is_nothrow_assignable<void, void>::value == false), "is_nothrow_assignable failure");
	static_assert((is_nothrow_assignable<int32_t, int32_t>::value == false), "is_nothrow_assignable failure");    // See is_assignable for why this is so.
	static_assert((is_nothrow_assignable<int32_t&, int32_t>::value == true), "is_nothrow_assignable failure");
	static_assert((is_nothrow_assignable<int32_t, int8_t>::value == false), "is_nothrow_assignable failure");
	static_assert((is_nothrow_assignable<int32_t&, int8_t>::value == true), "is_nothrow_assignable failure");
	static_assert((is_nothrow_assignable<NoThrowAssignable, NoThrowAssignable>::value == true), "is_nothrow_assignable failure");
//		static_assert((is_nothrow_assignable<ThrowAssignableTest, NoThrowAssignable>::value   == true),  "is_nothrow_assignable failure");
	static_assert((is_nothrow_assignable<ThrowAssignableTest, ThrowAssignableTest>::value == false), "is_nothrow_assignable failure");

	// is_bounded_array
	static_assert(is_bounded_array<void>::value == false, "is_bounded_array failure");
	static_assert(is_bounded_array<int>::value == false, "is_bounded_array failure");
	static_assert(is_bounded_array<PodA>::value == false, "is_bounded_array failure");
	static_assert(is_bounded_array<int[3]>::value == true, "is_bounded_array failure");
	static_assert(is_bounded_array<int[]>::value == false, "is_bounded_array failure");

	// is_unbounded_array
	static_assert(is_unbounded_array<void>::value == false, "is_bounded_array failure");
	static_assert(is_unbounded_array<int>::value == false, "is_bounded_array failure");
	static_assert(is_unbounded_array<PodA>::value == false, "is_bounded_array failure");
	static_assert(is_unbounded_array<int[3]>::value == false, "is_bounded_array failure");
	static_assert(is_unbounded_array<int[]>::value == true, "is_bounded_array failure");

	// is_trivially_copyable
	static_assert(is_trivially_copyable<void>::value == false, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<int>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<int*>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<int[]>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<int[4]>::value == true, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<NonPod1>::value == false, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<NoTrivialCopy1>::value == false, "is_trivially_copyable failure");
	static_assert(is_trivially_copyable<PodA>::value == true, "is_trivially_copyable failure");

	{  // user reported regression
		struct Foo {
			int a;
			Foo(int i) : a(i) {}
			Foo(Foo&& other) : a(other.a) { other.a = 0; }

			Foo(const Foo&) = delete;
			Foo& operator=(const Foo&) = delete;
		};

		static_assert(!is_trivially_copyable<Foo>::value, "is_trivially_copyable failure");
	}

	// is_trivially_copy_assignable
	static_assert(is_trivially_copy_assignable<int>::value == true, "is_trivially_copy_assignable failure");
	static_assert(is_trivially_copy_assignable<char*>::value == true, "is_trivially_copy_assignable failure");
	static_assert(is_trivially_copy_assignable<const char*>::value == true, "is_trivially_copy_assignable failure");
	static_assert(is_trivially_copy_assignable<NoTrivialCopy1>::value == false, "is_trivially_copy_assignable failure");

	// These tests currently fail on clang, but they would pass using the is_trivially_copy_assignable trait.  We should
	// determine if our implementation is correct, or if clang is actually incorrect.
//		static_assert(is_trivially_copy_assignable<const int>::value == true, "is_trivially_copy_assignable failure");
//		static_assert(is_trivially_copy_assignable<const PodA>::value == true, "is_trivially_copy_assignable failure");
	static_assert(is_trivially_copy_assignable<PodA>::value == true, "is_trivially_copy_assignable failure");

	// is_trivially_default_constructible
	// To do.


	// is_trivial
	// To do.


	// is_constructible
	static_assert(is_constructible<void>::value == false, "is_constructible failure");
	static_assert(is_constructible<const void>::value == false, "is_constructible failure");
	static_assert(is_constructible<int>::value == true, "is_constructible failure");
	static_assert(is_constructible<int&>::value == false, "is_constructible failure");
	static_assert(is_constructible<int&&>::value == false, "is_constructible failure");
	static_assert(is_constructible<int*>::value == true, "is_constructible failure");
	static_assert(is_constructible<int[]>::value == false, "is_constructible failure");
	static_assert(is_constructible<int[4]>::value == true, "is_constructible failure");
	static_assert(is_constructible<NonPod1>::value == true, " is_constructible failure");
	static_assert(is_constructible<NoTrivialCopy1>::value == true, "is_constructible failure");
	static_assert(is_constructible<PodA>::value == true, "is_constructible failure");
	static_assert(is_constructible<Abstract>::value == false, "is_constructible failure");
	static_assert(is_constructible<NonCopyable>::value == true, "is_constructible failure");
	static_assert((is_constructible<int, const int>::value == true), "is_constructible failure");
	static_assert((is_constructible<char*, const char*>::value == false), "is_constructible failure");
	static_assert((is_constructible<char*, char* const>::value == true), "is_constructible failure");
	static_assert((is_constructible<ThrowConstructibleTest, int>::value == true), "is_constructible failure");
	static_assert((is_constructible<ThrowConstructibleTest, float>::value == true), "is_constructible failure");

	// is_trivially_constructible
	// Need double parentheses because some older compilers need static_assert implemented as a macro.
	static_assert((is_trivially_constructible<void>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<void, void>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<void, int>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int>::value == true), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int, int>::value == true), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int, Abstract>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int*>::value == true), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int[]>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int[], int[]>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int[4]>::value == true), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<int[4], int[]>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<NoTrivialCopy1>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<PodA>::value == true), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<PodA, PodA>::value == true), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<Abstract>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<NonPod1>::value == false), "is_trivially_constructible failure");
	static_assert((is_trivially_constructible<NoTrivialConstructor>::value == false), "is_trivially_constructible failure");

	// is_nothrow_constructible
	static_assert((is_nothrow_constructible<void>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<int>::value == true), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<int*>::value == true), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<int[4]>::value == true), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<int[]>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<Abstract>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<int, const int>::value == true), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<char*, const char*>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<char*, char* const>::value == true), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<NonPod1>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<PodA>::value == true), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<ThrowConstructibleTest, int>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<ThrowConstructibleTest, float>::value == false), "is_nothrow_constructible failure");
	static_assert((is_nothrow_constructible<NoTrivialCopy1>::value == true), "is_nothrow_constructible failure");  //True because it's a compiler-generated constructor.

	// is_nothrow_move_constructible
	static_assert((is_nothrow_move_constructible<void>::value == false), "is_nothrow_move_constructible failure");
	static_assert((is_nothrow_move_constructible<int>::value == true), "is_nothrow_move_constructible failure");
	static_assert((is_nothrow_move_constructible<int*>::value == true), "is_nothrow_move_constructible failure");
	static_assert((is_nothrow_move_constructible<const int*>::value == true), "is_nothrow_move_constructible failure");
	static_assert((is_nothrow_move_constructible<int&>::value == true), "is_nothrow_move_constructible failure");
	static_assert((is_nothrow_move_constructible<double>::value == true), "is_nothrow_move_constructible failure");
	static_assert((is_nothrow_move_constructible<ClassEmpty>::value == true), "is_nothrow_move_constructible failure");

	// is_copy_constructible
	static_assert((is_copy_constructible<void>::value == false), "is_copy_constructible failure");
	static_assert((is_copy_constructible<int>::value == true), "is_copy_constructible failure");
	static_assert((is_copy_constructible<char*>::value == true), "is_copy_constructible failure");
	static_assert((is_copy_constructible<int&>::value == true),
				  "is_copy_constructible failure"); // As of this writing, GCC's libstdc++ reports true for this. I'm trying to find what's correct.
	static_assert((is_copy_constructible<const int>::value == true), "is_copy_constructible failure");
	static_assert((is_copy_constructible<HasTrivialCopy>::value == true), "is_copy_constructible failure");
	static_assert((is_copy_constructible<ConstructibleOnlyWithNonConstReference>::value == false), "is_copy_constructible failure");

	// is_destructible
	static_assert(is_destructible<int>::value == true, "is_destructible failure");
	static_assert(is_destructible<int&>::value == true, "is_destructible failure");
	static_assert(is_destructible<int&&>::value == true, "is_destructible failure");
	static_assert(is_destructible<char>::value == true, "is_destructible failure");
	static_assert(is_destructible<char*>::value == true, "is_destructible failure");
	static_assert(is_destructible<PodA>::value == true, "is_destructible failure");
	static_assert(is_destructible<void>::value == false, "is_destructible failure");
	static_assert(is_destructible<int[3]>::value == true, "is_destructible failure");
	static_assert(is_destructible<int[]>::value == false, "is_destructible failure"); // You can't call operator delete on this class.
	static_assert(is_destructible<Abstract>::value == true, "is_destructible failure");
	static_assert(is_destructible<AbstractWithDtor>::value == true, "is_destructible failure");
	static_assert(is_destructible<DeletedDtor>::value == false, "is_destructible failure"); // You can't call operator delete on this class.
	static_assert(is_destructible<NonPod2>::value == true, "is_destructible failure");

	// is_trivially_destructible
	static_assert(is_trivially_destructible<int>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<int&>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<int&&>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<char>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<char*>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<void>::value == false, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<PodA>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<int[3]>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<int[]>::value == false, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<Abstract>::value == true, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<AbstractWithDtor>::value == false, "is_trivially_destructible failure"); // Having a user-defined destructor make it non-trivial.
	static_assert(is_trivially_destructible<DeletedDtor>::value == false, "is_trivially_destructible failure");
	static_assert(is_trivially_destructible<NonPod2>::value == false,
				  "is_trivially_destructible failure");    // This case differs from is_destructible, because we have a declared destructor.

	// is_nothrow_destructible
	static_assert(is_nothrow_destructible<int>::value == true, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<int&>::value == true, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<int&&>::value == true, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<void>::value == false, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<Abstract>::value == true, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<AbstractWithDtor>::value == true, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<DeletedDtor>::value == false, "is_nothrow_destructible failure"); // You can't call operator delete on this class.
	static_assert(is_nothrow_destructible<NonPod2>::value == true,
				  "is_nothrow_destructible failure"); // NonPod2 is nothrow destructible because it has an empty destructor (makes no calls) which has no exception specification. Thus its exception specification defaults to noexcept(true) [C++11 Standard, 15.4 paragraph 14]
	static_assert(is_nothrow_destructible<NoThrowDestructible>::value == true, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<ThrowDestructible>::value == false, "is_nothrow_destructible failure");
	static_assert(is_nothrow_destructible<ThrowDestructibleNoexceptFalse>::value == true, "is_nothrow_destructible failure");

	// alignment_of
	static_assert(alignment_of<uint64_t>::value == 8, "alignment_of failure");
	static_assert(alignment_of<ClassAlign32>::value == 32, "alignment_of failure");

	// common_type
	static_assert((is_same<common_type<NonPod2*>::type, NonPod2*>::value));
	static_assert((is_same<common_type<int>::type, int>::value));
	static_assert((is_same<common_type<void, void>::type, void>::value));
	static_assert((is_same<common_type<int, int>::type, int>::value));

	// rank
	static_assert(rank<int[1][2][3][4][5][6]>::value == 6, "rank failure");
	static_assert(rank<int[][1][2]>::value == 3, "rank failure");
	static_assert(rank<int>::value == 0, "rank failure");
	static_assert(rank<void>::value == 0, "rank failure");

	static_assert(rank_v<int[1][2][3][4][5][6]> == 6, "rank failure");
	static_assert(rank_v<int[][1][2]> == 3, "rank failure");
	static_assert(rank_v<int> == 0, "rank failure");
	static_assert(rank_v<void> == 0, "rank failure");

	// extent
	static_assert((extent<int>::value == 0), "extent failure");
	static_assert((extent<int[2]>::value == 2), "extent failure");
	static_assert((extent<int[2][4]>::value == 2), "extent failure");
	static_assert((extent<int[]>::value == 0), "extent failure");
	static_assert((extent<int[][4]>::value == 0), "extent failure");
	static_assert((extent<int, 1>::value == 0), "extent failure");
	static_assert((extent<int[2], 1>::value == 0), "extent failure");
	static_assert((extent<int[2][4], 1>::value == 4), "extent failure");
	static_assert((extent<int[][4], 1>::value == 4), "extent failure");

	static_assert((extent_v<int> == 0), "extent failure");
	static_assert((extent_v<int[2]> == 2), "extent failure");
	static_assert((extent_v<int[2][4]> == 2), "extent failure");
	static_assert((extent_v<int[]> == 0), "extent failure");
	static_assert((extent_v<int[][4]> == 0), "extent failure");
	static_assert((extent_v<int, 1> == 0), "extent failure");
	static_assert((extent_v<int[2], 1> == 0), "extent failure");
	static_assert((extent_v<int[2][4], 1> == 4), "extent failure");
	static_assert((extent_v<int[][4], 1> == 4), "extent failure");

	// is_same
	static_assert((is_same<uint32_t, uint32_t>::value == true), "is_same failure");
	static_assert((is_same<void, void>::value == true), "is_same failure");
	static_assert((is_same<void*, void*>::value == true), "is_same failure");
	static_assert((is_same<uint64_t, uint64_t>::value == true), "is_same failure");
	static_assert((is_same<Class, Class>::value == true), "is_same failure");
	static_assert((is_same<uint64_t, uint32_t>::value == false), "is_same failure");
	static_assert((is_same<Class, ClassAlign32>::value == false), "is_same failure");

	static_assert((is_same_v<uint32_t, uint32_t> == true), "is_same_v failure");
	static_assert((is_same_v<void, void> == true), "is_same_v failure");
	static_assert((is_same_v<void*, void*> == true), "is_same_v failure");
	static_assert((is_same_v<uint64_t, uint64_t> == true), "is_same_v failure");
	static_assert((is_same_v<Class, Class> == true), "is_same_v failure");
	static_assert((is_same_v<uint64_t, uint32_t> == false), "is_same_v failure");
	static_assert((is_same_v<Class, ClassAlign32> == false), "is_same_v failure");

	// is_convertible
	static_assert((is_convertible<uint16_t, uint32_t>::value == true), "is_convertible failure");
	static_assert((is_convertible<int32_t, int16_t>::value == true),
				  "is_convertible failure");  // This is a conversion from 32 bits down to 16 bits. All compilers natively report that this is true. However, VC++ generates warnings for actual such conversions.
	static_assert((is_convertible<Subclass, Class>::value == true), "is_convertible failure");
	static_assert((is_convertible<Subclass*, Class*>::value == true), "is_convertible failure");
	static_assert((is_convertible<Subclass&, const Class&>::value == true), "is_convertible failure");
	static_assert((is_convertible<int, Class>::value == false), "is_convertible failure");
	static_assert((is_convertible<NonPod1, NonPod1>::value == true), "is_convertible failure");
	static_assert((is_convertible<NonPod1, NonPod2>::value == false), "is_convertible failure");
	static_assert((is_convertible<IsConvertibleTest1, IsConvertibleTest1>::value == false), "is_convertible failure");

	// make_signed
	// make_unsigned
	{
		// Test declarations
		make_signed<int8_t>::type i8 = -1;
		make_unsigned<uint8_t>::type u8 = 0xff;

		make_signed<int16_t>::type i16 = -1;
		make_unsigned<uint16_t>::type u16 = 0xffff;

		make_signed<int32_t>::type i32 = -1;
		make_unsigned<uint32_t>::type u32 = 0xffffffff;

		make_signed<int64_t>::type i64 = -1;
		make_unsigned<uint64_t>::type u64 = UINT64_C(0xffffffffffffffff);

		// Test conversions via static_cast:
		u8 = static_cast<make_unsigned<uint8_t>::type>(i8);
		i8 = static_cast<make_signed<int8_t>::type>(u8);

		u16 = static_cast<make_unsigned<uint16_t>::type>(i16);
		i16 = static_cast<make_signed<int16_t>::type>(u16);

		u32 = static_cast<make_unsigned<uint32_t>::type>(i32);
		i32 = static_cast<make_signed<int32_t>::type>(u32);

		u64 = static_cast<make_unsigned<uint64_t>::type>(i64);
		i64 = static_cast<make_signed<int64_t>::type>(u64);

		static_assert(is_same_v<signed char, make_signed<unsigned char>::type>);
		static_assert(is_same_v<short, make_signed<unsigned short>::type>);
		static_assert(is_same_v<int, make_signed<unsigned int>::type>);
		static_assert(is_same_v<long, make_signed<unsigned long>::type>);
		static_assert(is_same_v<long long, make_signed<unsigned long long>::type>);

		static_assert(is_same_v<const signed char, make_signed<const unsigned char>::type>);
		static_assert(is_same_v<const short, make_signed<const unsigned short>::type>);
		static_assert(is_same_v<const int, make_signed<const unsigned int>::type>);
		static_assert(is_same_v<const long, make_signed<const unsigned long>::type>);
		static_assert(is_same_v<const long long, make_signed<const unsigned long long>::type>);

		static_assert(is_same_v<volatile signed char, make_signed<volatile unsigned char>::type>);
		static_assert(is_same_v<volatile short, make_signed<volatile unsigned short>::type>);
		static_assert(is_same_v<volatile int, make_signed<volatile unsigned int>::type>);
		static_assert(is_same_v<volatile long, make_signed<volatile unsigned long>::type>);
		static_assert(is_same_v<volatile long long, make_signed<volatile unsigned long long>::type>);

		static_assert(is_same_v<const volatile signed char, make_signed<const volatile unsigned char>::type>);
		static_assert(is_same_v<const volatile short, make_signed<const volatile unsigned short>::type>);
		static_assert(is_same_v<const volatile int, make_signed<const volatile unsigned int>::type>);
		static_assert(is_same_v<const volatile long, make_signed<const volatile unsigned long>::type>);
		static_assert(is_same_v<const volatile long long, make_signed<const volatile unsigned long long>::type>);

		static_assert(is_same_v<unsigned char, make_unsigned<signed char>::type>);
		static_assert(is_same_v<unsigned short, make_unsigned<short>::type>);
		static_assert(is_same_v<unsigned int, make_unsigned<int>::type>);
		static_assert(is_same_v<unsigned long, make_unsigned<long>::type>);
		static_assert(is_same_v<unsigned long long, make_unsigned<long long>::type>);

		static_assert(is_same_v<const unsigned char, make_unsigned<const signed char>::type>);
		static_assert(is_same_v<const unsigned short, make_unsigned<const short>::type>);
		static_assert(is_same_v<const unsigned int, make_unsigned<const int>::type>);
		static_assert(is_same_v<const unsigned long, make_unsigned<const long>::type>);
		static_assert(is_same_v<const unsigned long long, make_unsigned<const long long>::type>);

		static_assert(is_same_v<volatile unsigned char, make_unsigned<volatile signed char>::type>);
		static_assert(is_same_v<volatile unsigned short, make_unsigned<volatile short>::type>);
		static_assert(is_same_v<volatile unsigned int, make_unsigned<volatile int>::type>);
		static_assert(is_same_v<volatile unsigned long, make_unsigned<volatile long>::type>);
		static_assert(is_same_v<volatile unsigned long long, make_unsigned<volatile long long>::type>);

		static_assert(is_same_v<const volatile unsigned char, make_unsigned<const volatile signed char>::type>);
		static_assert(is_same_v<const volatile unsigned short, make_unsigned<const volatile short>::type>);
		static_assert(is_same_v<const volatile unsigned int, make_unsigned<const volatile int>::type>);
		static_assert(is_same_v<const volatile unsigned long, make_unsigned<const volatile long>::type>);
		static_assert(is_same_v<const volatile unsigned long long, make_unsigned<const volatile long long>::type>);

		static_assert(is_same_v<signed char, make_signed<signed char>::type>);
		static_assert(is_same_v<short, make_signed<signed short>::type>);
		static_assert(is_same_v<int, make_signed<signed int>::type>);
		static_assert(is_same_v<long, make_signed<signed long>::type>);
		static_assert(is_same_v<long long, make_signed<signed long long>::type>);

		static_assert(is_same_v<unsigned char, make_unsigned<unsigned char>::type>);
		static_assert(is_same_v<unsigned short, make_unsigned<unsigned short>::type>);
		static_assert(is_same_v<unsigned int, make_unsigned<unsigned int>::type>);
		static_assert(is_same_v<unsigned long, make_unsigned<unsigned long>::type>);
		static_assert(is_same_v<unsigned long long, make_unsigned<unsigned long long>::type>);

		// Char tests
		static_assert(sizeof(char) == sizeof(make_signed<char>::type));
		static_assert(sizeof(wchar_t) == sizeof(make_signed<wchar_t>::type));
		static_assert(sizeof(char8_t) == sizeof(make_signed<char8_t>::type));
		static_assert(sizeof(char16_t) == sizeof(make_signed<char16_t>::type));
		static_assert(sizeof(char32_t) == sizeof(make_signed<char32_t>::type));
		static_assert(sizeof(char) == sizeof(make_unsigned<char>::type));
		static_assert(sizeof(wchar_t) == sizeof(make_unsigned<wchar_t>::type));
		static_assert(sizeof(char8_t) == sizeof(make_unsigned<char8_t>::type));
		static_assert(sizeof(char16_t) == sizeof(make_unsigned<char16_t>::type));
		static_assert(sizeof(char32_t) == sizeof(make_unsigned<char32_t>::type));

		static_assert(is_same_v<signed char, make_signed<char8_t>::type>);
		static_assert(is_same_v<unsigned char, make_unsigned<char8_t>::type>);

		// Enum tests
		enum EnumUCharSize : unsigned char {};
		enum EnumUShortSize : unsigned short {};
		enum EnumUIntSize : unsigned int {};
		enum EnumULongSize : unsigned long {};
		enum EnumULongLongSize : unsigned long long {};

		static_assert(is_signed_v<make_signed<EnumUCharSize>::type>);
		static_assert(is_signed_v<make_signed<EnumUShortSize>::type>);
		static_assert(is_signed_v<make_signed<EnumUIntSize>::type>);
		static_assert(is_signed_v<make_signed<EnumULongSize>::type>);
		static_assert(is_signed_v<make_signed<EnumULongLongSize>::type>);
		static_assert(sizeof(EnumUCharSize) == sizeof(make_signed<EnumUCharSize>::type));
		static_assert(sizeof(EnumUShortSize) == sizeof(make_signed<EnumUShortSize>::type));
		static_assert(sizeof(EnumUIntSize) == sizeof(make_signed<EnumUIntSize>::type));
		static_assert(sizeof(EnumULongSize) == sizeof(make_signed<EnumULongSize>::type));
		static_assert(sizeof(EnumULongLongSize) == sizeof(make_signed<EnumULongLongSize>::type));

		enum EnumCharSize : signed char {};
		enum EnumShortSize : short {};
		enum EnumIntSize : int {};
		enum EnumLongSize : long {};
		enum EnumLongLongSize : long long {};

		static_assert(is_unsigned_v<make_unsigned<EnumCharSize>::type>);
		static_assert(is_unsigned_v<make_unsigned<EnumShortSize>::type>);
		static_assert(is_unsigned_v<make_unsigned<EnumIntSize>::type>);
		static_assert(is_unsigned_v<make_unsigned<EnumLongSize>::type>);
		static_assert(is_unsigned_v<make_unsigned<EnumLongLongSize>::type>);
		static_assert(sizeof(EnumCharSize) == sizeof(make_unsigned<EnumCharSize>::type));
		static_assert(sizeof(EnumShortSize) == sizeof(make_unsigned<EnumShortSize>::type));
		static_assert(sizeof(EnumIntSize) == sizeof(make_unsigned<EnumIntSize>::type));
		static_assert(sizeof(EnumLongSize) == sizeof(make_unsigned<EnumLongSize>::type));
		static_assert(sizeof(EnumLongLongSize) == sizeof(make_unsigned<EnumLongLongSize>::type));
	}

	// remove_const
	// remove_volatile
	// remove_cv
	{
		// To do: Make more thorough tests verifying this. Such tests will probably involve template metaprogramming.
		remove_const<const int32_t>::type i32 = 47;

		remove_volatile<volatile int16_t>::type i16 = 47;

		remove_cv<const volatile int32_t>::type i64 = 47;

	}

	// remove_cvref
	{
		static_assert(is_same_v<remove_cvref_t<int>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int&>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int&&>, int>, "remove_cvref failure");

		static_assert(is_same_v<remove_cvref_t<const int>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<const int&>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<const int&&>, int>, "remove_cvref failure");

		static_assert(is_same_v<remove_cvref_t<volatile int>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<volatile int&>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<volatile int&&>, int>, "remove_cvref failure");

		static_assert(is_same_v<remove_cvref_t<const volatile int>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<const volatile int&>, int>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<const volatile int&&>, int>, "remove_cvref failure");

		// test pointer types
		static_assert(is_same_v<remove_cvref_t<int*>, int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int*&>, int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int*&&>, int*>, "remove_cvref failure");

		static_assert(is_same_v<remove_cvref_t<const int*>, const int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<const int*&>, const int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<const int*&&>, const int*>, "remove_cvref failure");

		static_assert(is_same_v<remove_cvref_t<int* const>, int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int* const&>, int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int* const&&>, int*>, "remove_cvref failure");

		static_assert(is_same_v<remove_cvref_t<int* const volatile>, int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int* const volatile&>, int*>, "remove_cvref failure");
		static_assert(is_same_v<remove_cvref_t<int* const volatile&&>, int*>, "remove_cvref failure");
	}

	// add_const
	// add_volatile
	// add_cv
	{
		// To do: Make more thorough tests verifying this. Such tests will probably involve template metaprogramming.
		add_const<int32_t>::type i32 = 47;

		add_volatile<volatile int16_t>::type i16 = 47;

		add_cv<const volatile int32_t>::type i64 = 47;
	}

	// remove_reference
	// add_reference
	// remove_pointer
	// add_pointer
	// remove_extent
	// remove_all_extents
	{
		int x = 17;

		remove_reference<int&>::type xValue;
		xValue = 3;

		add_pointer<int>::type xPtr = &x;
		*xPtr = 19;

		remove_pointer<int*>::type yValue;
		yValue = 3;

		// ref to T
		//   -> T*
		static_assert(is_same_v<add_pointer_t<int&>, int*>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<int (&)()>, int (*)()>, "add_pointer failure");

		// object type (a (possibly cv-qualified) type other than function type, reference type or void), or
		// a function type that is not cv- or ref-qualified, or a (possibly cv-qualified) void type
		//   -> T*
		static_assert(is_same_v<add_pointer_t<int>, int*>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<int*>, int**>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<int()>, int (*)()>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<void>, void*>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<const void>, const void*>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<volatile void>, volatile void*>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<const volatile void>, const volatile void*>, "add_pointer failure");

		// otherwise (cv- or ref-qualified function type)
		//   -> T
		static_assert(is_same_v<add_pointer_t<int() const>, int() const>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<int() volatile>, int() volatile>, "add_pointer failure");
		static_assert(is_same_v<add_pointer_t<int() const volatile>, int() const volatile>, "add_pointer failure");

		// remove_extent
		// If T is an array of some type X, provides the member typedef type equal to X, otherwise 
		// type is T. Note that if T is a multidimensional array, only the first dimension is removed. 
		typedef int IntArray1[37];
		typedef remove_extent<IntArray1>::type Int;
		static_assert((is_same<Int, int>::value == true), "remove_extent/is_same failure");

		// remove_all_extents
		typedef int IntArray2[37][54];
		typedef remove_all_extents<IntArray2>::type Int2;
		static_assert((is_same<Int2, int>::value == true), "remove_all_extents/is_same failure");
	}

	// add_lvalue_reference
	{
		// function type with no cv- or ref-qualifier
		//   -> T&
		static_assert(is_same_v<add_lvalue_reference_t<void()>, void (&)()>, "add_lvalue_reference failure");

		// object type (a (possibly cv-qualified) type other than function type, reference type or void)
		//   -> T&
		static_assert(is_same_v<add_lvalue_reference_t<int>, int&>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<const int>, const int&>, "add_lvalue_reference failure");

		// if T is an rvalue reference (to some type U)
		//   -> U&
		static_assert(is_same_v<add_lvalue_reference_t<int&&>, int&>, "add_lvalue_reference failure");

		// otherwise (cv- or ref-qualified function type, or reference type, or (possibly cv-qualified) void)
		//   -> T
		static_assert(is_same_v<add_lvalue_reference_t<void() const>, void() const>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<void()&>, void()&>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<void()&&>, void()&&>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<int&>, int&>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<const int&>, const int&>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<void>, void>, "add_lvalue_reference failure");
		static_assert(is_same_v<add_lvalue_reference_t<const void>, const void>, "add_lvalue_reference failure");
	}

	// add_rvalue_reference
	{
		// function type with no cv- or ref-qualifier
		//   -> T&&
		static_assert(is_same_v<add_rvalue_reference_t<void()>, void (&&)()>, "add_rvalue_reference failure");

		// object type (a (possibly cv-qualified) type other than function type, reference type or void)
		//   -> T&&
		static_assert(is_same_v<add_rvalue_reference_t<int>, int&&>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<const int>, const int&&>, "add_rvalue_reference failure");

		// otherwise (cv- or ref-qualified function type, or reference type, or (possibly cv-qualified) void)
		//   -> T
		static_assert(is_same_v<add_rvalue_reference_t<void() const>, void() const>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<void()&>, void()&>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<void()&&>, void()&&>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<int&>, int&>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<int&&>, int&&>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<const int&>, const int&>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<void>, void>, "add_rvalue_reference failure");
		static_assert(is_same_v<add_rvalue_reference_t<const void>, const void>, "add_rvalue_reference failure");
	}

	// decay
	{
		static_assert((is_same<uint32_t, decay<uint32_t>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t, decay<const uint32_t>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t, decay<volatile uint32_t>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t, decay<uint32_t&>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t, decay<const uint32_t&>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t, decay<const volatile uint32_t&>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t, decay<uint32_t&&>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t*, decay<uint32_t[3]>::type>::value == true), "is_same failure");
		static_assert((is_same<uint32_t(*)(char), decay<uint32_t(char)>::type>::value == true), "is_same failure");
	}

	// void_t
	{
		{
			static_assert(is_same<void_t<void>, void>::value, "void_t failure");
			static_assert(is_same<void_t<int>, void>::value, "void_t failure");
			static_assert(is_same<void_t<short>, void>::value, "void_t failure");
			static_assert(is_same<void_t<long>, void>::value, "void_t failure");
			static_assert(is_same<void_t<long long>, void>::value, "void_t failure");
			static_assert(is_same<void_t<ClassEmpty>, void>::value, "void_t failure");
			static_assert(is_same<void_t<ClassNonEmpty>, void>::value, "void_t failure");
			static_assert(is_same<void_t<std::vector<int>>, void>::value, "void_t failure");
		}

		// new sfinae mechansim test 
		{
			static_assert(has_increment_operator_using_void_t<HasIncrementOperator>::value, "void_t sfinae failure");
			static_assert(!has_increment_operator_using_void_t<ClassEmpty>::value, "void_t sfinae failure");
		}
	}

	// conjunction
	{
		static_assert(conjunction<>::value, "conjunction failure");
		static_assert(!conjunction<false_type>::value, "conjunction failure");
		static_assert(!conjunction<false_type, false_type>::value, "conjunction failure");
		static_assert(!conjunction<false_type, false_type, false_type>::value, "conjunction failure");
		static_assert(!conjunction<false_type, false_type, false_type, true_type>::value, "conjunction failure");
		static_assert(!conjunction<false_type, false_type, true_type, true_type>::value, "conjunction failure");
		static_assert(!conjunction<false_type, true_type, true_type, true_type>::value, "conjunction failure");
		static_assert(!conjunction<true_type, true_type, true_type, true_type, false_type>::value, "conjunction failure");
		static_assert(!conjunction<true_type, false_type, true_type, true_type, true_type>::value, "conjunction failure");
		static_assert(conjunction<true_type, true_type, true_type, true_type, true_type>::value, "conjunction failure");
		static_assert(conjunction<true_type, true_type, true_type, true_type>::value, "conjunction failure");
		static_assert(conjunction<true_type, true_type, true_type>::value, "conjunction failure");
		static_assert(conjunction<true_type>::value, "conjunction failure");

		static_assert(conjunction_v<>, "conjunction failure");
		static_assert(!conjunction_v<false_type>, "conjunction failure");
		static_assert(!conjunction_v<false_type, false_type>, "conjunction failure");
		static_assert(!conjunction_v<false_type, false_type, false_type>, "conjunction failure");
		static_assert(!conjunction_v<false_type, false_type, false_type, true_type>, "conjunction failure");
		static_assert(!conjunction_v<false_type, false_type, true_type, true_type>, "conjunction failure");
		static_assert(!conjunction_v<false_type, true_type, true_type, true_type>, "conjunction failure");
		static_assert(!conjunction_v<true_type, true_type, true_type, true_type, false_type>, "conjunction failure");
		static_assert(!conjunction_v<true_type, false_type, true_type, true_type, true_type>, "conjunction failure");
		static_assert(conjunction_v<true_type, true_type, true_type, true_type, true_type>, "conjunction failure");
		static_assert(conjunction_v<true_type, true_type, true_type, true_type>, "conjunction failure");
		static_assert(conjunction_v<true_type, true_type, true_type>, "conjunction failure");
		static_assert(conjunction_v<true_type>, "conjunction failure");
	}

	// disjunction
	{
		static_assert(!disjunction<>::value, "disjunction failure");
		static_assert(!disjunction<false_type>::value, "disjunction failure");
		static_assert(!disjunction<false_type, false_type>::value, "disjunction failure");
		static_assert(!disjunction<false_type, false_type, false_type>::value, "disjunction failure");
		static_assert(disjunction<false_type, false_type, false_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<false_type, false_type, true_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<false_type, true_type, true_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<true_type, true_type, true_type, true_type, false_type>::value, "disjunction failure");
		static_assert(disjunction<true_type, false_type, true_type, true_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<true_type, true_type, true_type, true_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<true_type, true_type, true_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<true_type, true_type, true_type>::value, "disjunction failure");
		static_assert(disjunction<true_type>::value, "disjunction failure");

		static_assert(!disjunction_v<>, "disjunction failure");
		static_assert(!disjunction_v<false_type>, "disjunction failure");
		static_assert(!disjunction_v<false_type, false_type>, "disjunction failure");
		static_assert(!disjunction_v<false_type, false_type, false_type>, "disjunction failure");
		static_assert(disjunction_v<false_type, false_type, false_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<false_type, false_type, true_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<false_type, true_type, true_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<true_type, true_type, true_type, true_type, false_type>, "disjunction failure");
		static_assert(disjunction_v<true_type, false_type, true_type, true_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<true_type, true_type, true_type, true_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<true_type, true_type, true_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<true_type, true_type, true_type>, "disjunction failure");
		static_assert(disjunction_v<true_type>, "disjunction failure");
	}

	// negation
	{
		static_assert(negation<false_type>::value, "negation failure");
		static_assert(!negation<true_type>::value, "negation failure");
		static_assert(negation_v<false_type>, "negation failure");
		static_assert(!negation_v<true_type>, "negation failure");
	}

	// has_unique_object_representations
	{
		static_assert(has_unique_object_representations<bool>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<char16_t>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<char32_t>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<char>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<int>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<long long>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<long>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<short>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<signed char>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<unsigned char>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<unsigned int>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<unsigned long long>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<unsigned long>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<unsigned short>::value, "has_unique_object_representations failure");
		static_assert(!has_unique_object_representations<void>::value, "has_unique_object_representations failure");
		static_assert(has_unique_object_representations<wchar_t>::value, "has_unique_object_representations failure");

		{
			struct packed_type { int a; };
			static_assert(has_unique_object_representations<packed_type>::value, "has_unique_object_representations failure");

			struct padded_type { int a; char b; int c; };
			static_assert(!has_unique_object_representations<padded_type>::value, "has_unique_object_representations failure");
		}
	}

	// is_final
	{
		static_assert(is_final<FinalStruct>::value == is_final<FinalStruct>::value, "final struct not correctly detected");
		static_assert(is_final<FinalClass>::value == is_final<FinalClass>::value, "final class not correctly detected");
		static_assert(is_final<Enum>::value == is_final<Enum>::value, "enum not correctly detected");
		static_assert(is_final<int>::value == is_final<int>::value, "int not correctly detected");
		static_assert(is_final<Struct>::value == is_final<Struct>::value, "non-final struct not correctly detected");
		static_assert(is_final<Class>::value == is_final<Class>::value, "non-final class not correctly detected");
	}

	// is_aggregate
	{
		static_assert(!is_aggregate_v<int>, "is_aggregate failure");
		static_assert(is_aggregate_v<int[]>, "is_aggregate failure");

		{
			struct Aggregrate {};
			static_assert(is_aggregate_v<Aggregrate>, "is_aggregate failure");
		}

		{
			struct NotAggregrate { NotAggregrate() {}}; // user provided ctor
			static_assert(!is_aggregate_v<NotAggregrate>, "is_aggregate failure");
		}

		{
			struct NotAggregrate { virtual void foo() {}}; // virtual member function
			static_assert(!is_aggregate_v<NotAggregrate>, "is_aggregate failure");
		}
	}
}