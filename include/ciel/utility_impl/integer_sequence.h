#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_INTEGER_SEQUENCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_INTEGER_SEQUENCE_H_

#include <ciel/type_traits_impl/is_integral.h>
#include <cstddef>

namespace ciel {

	template<class T, T... Ints>
		requires ciel::is_integral_v<T>
	struct integer_sequence {
		using value_type = T;

		static constexpr size_t size() noexcept {
			return sizeof...(Ints);
		}

		using type = integer_sequence;    // 为了 make_index_sequence 的代码实现所设
	};

	template<size_t... Ints>
	using index_sequence = integer_sequence<size_t, Ints...>;

	namespace integer_sequence_details {
		/*
		make_index_sequence 的实现来源：https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence
		这个实现最妙的地方在于通过 merge_and_renumber 的偏特化 index_sequence<I...> 从而在模板形参里取出了 size_t... I 的形参包
		这样比如 I1... 为 [0, 1]
		 		I2... 为 [0, 1, 2]
		 		I1..., (sizeof...(I1) + I2)... 就刚好变成了 [0, 1, 2, 3, 4]
		然后通过 N / 2 的切分还实现了 O(log N) 的复杂度
		*/
		template<class Sequence1, class Sequence2>
		struct merge_and_renumber;

		template<size_t... I1, size_t... I2>
		struct merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};

		template<size_t N>
		struct make_index_sequence_helper : merge_and_renumber<typename make_index_sequence_helper<N / 2>::type, typename make_index_sequence_helper<N - N / 2>::type> {};

		template<>
		struct make_index_sequence_helper<0> : index_sequence<> {};
		template<>
		struct make_index_sequence_helper<1> : index_sequence<0> {};
	}

	template<size_t N>
	using make_index_sequence = typename integer_sequence_details::make_index_sequence_helper<N>::type;

	namespace integer_sequence_details {
		// 借用 make_index_sequence 的经验和成果，做个简单的强转
		template<typename T, size_t N, typename Indices = make_index_sequence<N>>
		struct make_integer_sequence_helper;

		template<typename T, size_t N, size_t... Indices>
		struct make_integer_sequence_helper<T, N, index_sequence<Indices...>> {
			using type = integer_sequence<T, static_cast<T>(Indices)...>;
		};
	}

	template<class T, T N>
	using make_integer_sequence = typename integer_sequence_details::make_integer_sequence_helper<T, N>::type;

	template<class... T>
	using index_sequence_for = make_index_sequence<sizeof...(T)>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_INTEGER_SEQUENCE_H_