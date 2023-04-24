#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_

#include <cstddef>

namespace ciel {

//remove_reference
	template<class T>
	struct remove_reference {
		using type = T;
	};

	template<class T>
	struct remove_reference<T&> {
		using type = T;
	};

	template<class T>
	struct remove_reference<T&&> {
		using type = T;
	};

	template<class T>
	using remove_reference_t = typename remove_reference<T>::type;

//remove_cv
	template<class T>
	struct remove_cv {
		using type = T;
	};

	template<class T>
	struct remove_cv<const T> {
		using type = T;
	};

	template<class T>
	struct remove_cv<volatile T> {
		using type = T;
	};

	template<class T>
	struct remove_cv<const volatile T> {
		using type = T;
	};

	template<class T>
	struct remove_const {
		using type = T;
	};

	template<class T>
	struct remove_const<const T> {
		using type = T;
	};

	template<class T>
	struct remove_volatile {
		using type = T;
	};

	template<class T>
	struct remove_volatile<volatile T> {
		using type = T;
	};

	template<class T>
	using remove_cv_t = typename remove_cv<T>::type;

	template<class T>
	using remove_const_t = typename remove_const<T>::type;

	template<class T>
	using remove_volatile_t = typename remove_volatile<T>::type;

//remove_pointer
	template<class T>
	struct remove_pointer {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T*> {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T* const> {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T* volatile> {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T* const volatile> {
		using type = T;
	};

	template<class T>
	using remove_pointer_t = typename remove_pointer<T>::type;

//conditional
	template<bool B, class T, class F>
	struct conditional {
		using type = T;
	};

	template<class T, class F>
	struct conditional<false, T, F> {
		using type = F;
	};

	template<bool B, class T, class F>
	using conditional_t = typename conditional<B, T, F>::type;

//integral_constant
	template<class T, T v>
	struct integral_constant {
		static constexpr T value = v;
		using value_type = T;
		using type = integral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}
		constexpr value_type operator()() const noexcept {
			return value;
		}
	};

	template<bool B>
	using bool_constant = integral_constant<bool, B>;

	using true_type = integral_constant<bool, true>;
	using false_type = integral_constant<bool, false>;

//is_same
	template<class T, class U>
	struct is_same : false_type {};

	template<class T>
	struct is_same<T, T> : true_type {};

	template<class T, class U>
	inline constexpr bool is_same_v = is_same<T, U>::value;

//is_array
	template<class T>
	struct is_array : false_type {};

	template<class T>
	struct is_array<T[]> : true_type {};

	template<class T, size_t N>
	struct is_array<T[N]> : true_type {};

	template<class T>
	inline constexpr bool is_array_v = is_array<T>::value;

//remove_extent
	template<class T>
	struct remove_extent {
		using type = T;
	};

	template<class T>
	struct remove_extent<T[]> {
		using type = T;
	};

	template<class T, size_t N>
	struct remove_extent<T[N]> {
		using type = T;
	};

	template<class T>
	using remove_extent_t = typename remove_extent<T>::type;

//is_const
	template<class T>
	struct is_const : false_type {};

	template<class T>
	struct is_const<const T> : true_type {};

	template<class T>
	inline constexpr bool is_const_v = is_const<T>::value;

//is_pointer
	template<class T>
	struct is_pointer_helper : false_type {};

	template<class T>
	struct is_pointer_helper<T*> : true_type {};

	template<class T>
	struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};

	template<class T>
	inline constexpr bool is_pointer_v = is_pointer<T>::value;

//is_reference
	template<class T>
	struct is_reference : false_type {};

	template<class T>
	struct is_reference<T&> : true_type {};

	template<class T>
	struct is_reference<T&&> : true_type {};

	template<class T>
	inline constexpr bool is_reference_v = is_reference<T>::value;

//is_function
	template<class T>
	struct is_function : bool_constant<!is_const_v<const T> && !is_reference_v<T>> {};

	template<class T>
	inline constexpr bool is_function_v = is_function<T>::value;

//type_identity
	template<class T>
	struct type_identity {
		using type = T;
	};

	template<class T>
	using type_identity_t = typename type_identity<T>::type;

//remove_cvref
	template<class T>
	struct remove_cvref {
		typedef remove_cv_t<remove_reference_t<T>> type;
	};

	template<class T>
	using remove_cvref_t = typename remove_cvref<T>::type;

//add_pointer
	template<class T>
	struct add_pointer : type_identity<remove_reference_t<T>*> {};

	template<class T>
	using add_pointer_t = typename add_pointer<T>::type;

//add_lvalue_reference
	template<class T>
	struct add_lvalue_reference : type_identity<T&> {};

	template<class T>
	using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

//add_rvalue_reference
	template<class T>
	struct add_rvalue_reference : type_identity<T&&> {};

	template<class T>
	using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

//is_lvalue_reference
	template<class T>
	struct is_lvalue_reference : false_type {};

	template<class T>
	struct is_lvalue_reference<T&> : true_type {};

	template<class T>
	inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

//is_rvalue_reference
	template<class T>
	struct is_rvalue_reference : false_type {};

	template<class T>
	struct is_rvalue_reference<T&&> : true_type {};

	template<class T>
	inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

//decay
	template<class T>
	class decay {
	private:
		using U = remove_reference_t<T>;

	public:
		using type = conditional_t<is_array_v<U>,
								   remove_extent_t<U>*,
								   conditional_t<is_function_v<U>, add_pointer_t<U>, remove_cvref<U>>>;
	};

	template<class T>
	using decay_t = typename decay<T>::type;

//is_constructible
	template<class T, class... Args>
	struct is_constructible;

//is_default_constructible
	template<class T>
	struct is_default_constructible {};

	//common_type
//	template<class... T>
//	struct common_type{
//
//	};
//
//	template< class... T >
//	using common_type_t = typename common_type<T...>::type;

}    //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_H_