#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_PRINT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_PRINT_H_

#include <iostream>
#include <ciel/type_traits.h>
#include <ciel/utility.h>
#include <ciel/tuple.h>

namespace ciel {

	// FIXME: 由于打印结构体是用聚合初始化获取成员个数，再用结构化绑定获取每个成员，所以数组在结构体里会影响这两个功能而导致出错，目前没有好的解决办法

	constexpr void println(const auto& t, size_t tab = 0);

	namespace print_details {
		struct any_type {
			template<class T>
			operator T();
		};

		template<class T>
		constexpr size_t struct_member_number(auto&& ...args) {
			if constexpr (!requires{ T{args...}; }) {
				return sizeof...(args) - 1;
			} else {
				return struct_member_number<T>(args..., any_type{});
			}
		}
	}

	// 平凡结构体
	template<class T>
		requires ciel::is_aggregate_v<T>
	constexpr void print(const T& t, size_t tab = 0) {
		constexpr size_t smn = print_details::struct_member_number<T>();
		static_assert(smn < 9, "ciel::println 暂不支持成员超过 8 个的结构体");
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
		std::cout << "{\n";
		if constexpr (smn == 0) {
			std::cout << "这里是一个空结构体。\n";
		} else if constexpr (smn == 1) {
			auto&& [a1] = t;
			println(a1, tab + 1);
		} else if constexpr (smn == 2) {
			auto&& [a1, a2] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
		} else if constexpr (smn == 3) {
			auto&& [a1, a2, a3] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
			println(a3, tab + 1);
		} else if constexpr (smn == 4) {
			auto&& [a1, a2, a3, a4] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
			println(a3, tab + 1);
			println(a4, tab + 1);
		} else if constexpr (smn == 5) {
			auto&& [a1, a2, a3, a4, a5] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
			println(a3, tab + 1);
			println(a4, tab + 1);
			println(a5, tab + 1);
		} else if constexpr (smn == 6) {
			auto&& [a1, a2, a3, a4, a5, a6] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
			println(a3, tab + 1);
			println(a4, tab + 1);
			println(a5, tab + 1);
			println(a6, tab + 1);
		} else if constexpr (smn == 7) {
			auto&& [a1, a2, a3, a4, a5, a6, a7] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
			println(a3, tab + 1);
			println(a4, tab + 1);
			println(a5, tab + 1);
			println(a6, tab + 1);
			println(a7, tab + 1);
		} else {
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8] = t;
			println(a1, tab + 1);
			println(a2, tab + 1);
			println(a3, tab + 1);
			println(a4, tab + 1);
			println(a5, tab + 1);
			println(a6, tab + 1);
			println(a7, tab + 1);
			println(a8, tab + 1);
		}
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
		std::cout << "}";
	}

	// 基础类型 / string / 被用户定义过 cout 重载的类型
	template<class T>
		requires requires(T t) { std::cout << t; }
	constexpr void print(const T& t, size_t tab = 0) {
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
        if constexpr (ciel::is_same_v<ciel::remove_cvref_t<T>, bool>) {
            std::cout << std::boolalpha << t;
        } else {
            std::cout << t;
        }
	}

	// pair
	template<class T>
		requires requires(T t) { t.first; t.second; } && (!requires(T t) { std::cout << t; })
	constexpr void print(const T& t, size_t tab = 0) {
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
		std::cout << "[  ";
        print(t.first);
        std::cout << "  ";
        print(t.second);
        std::cout << "  ]";
	}

	// 可 range based for loop 的容器
	template<class T>
		requires requires(T t) { t.begin(); t.end(); } && (!requires(T t) { std::cout << t; })
	constexpr void print(const T& t, size_t tab = 0) {
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
		std::cout << "{  ";
		for (auto& i : t) {
			print(i, 0);
			std::cout << "  ";
		}
		std::cout << "}";
	}

	// 数组
	template<class T, size_t S>
	constexpr void print(T(& t)[S], size_t tab = 0) {
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
		std::cout << "[  ";
		for (const auto& i : t) {
			print(i, 0);
			std::cout << "  ";
		}
		std::cout << "]";
	}

	// tuple
	namespace print_details {
		template<class... Types, size_t... I>
		constexpr void print_tuple_helper(const std::tuple<Types...>& t, ciel::index_sequence<I...>) {
			((print(std::get<I>(t)), std::cout << "  "), ...);
		}
        template<class... Types, size_t... I>
        constexpr void print_tuple_helper(const ciel::tuple<Types...>& t, ciel::index_sequence<I...>) {
            ((print(ciel::get<I>(t)), std::cout << "  "), ...);
        }
	}

	template<class... Types>
	constexpr void print(const std::tuple<Types...>& t, size_t tab = 0) {
		for (size_t i = 0; i < tab; ++i) {
			std::cout << "    ";
		}
		std::cout << "[  ";
		print_details::print_tuple_helper(t, ciel::index_sequence_for<Types...>());
		std::cout << "]";
	}
    template<class... Types>
    constexpr void print(const ciel::tuple<Types...>& t, size_t tab = 0) {
        for (size_t i = 0; i < tab; ++i) {
            std::cout << "    ";
        }
        std::cout << "[  ";
        print_details::print_tuple_helper(t, ciel::index_sequence_for<Types...>());
        std::cout << "]";
    }

	constexpr void println(const auto& t, size_t tab) {
		print(t, tab);
		std::cout << '\n';
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_PRINT_H_