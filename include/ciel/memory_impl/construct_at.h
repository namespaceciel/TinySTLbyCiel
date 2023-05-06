#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_CONSTRUCT_AT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_CONSTRUCT_AT_H_

#include <ciel/utility.h>

namespace ciel {

	template<class T, class... Args, class = decltype(::new(declval<void*>()) T(declval<Args>()...))>
	constexpr T* construct_at(T* p, Args&& ... args) {
		static_assert(p != nullptr, "你他妈传空指针进来？");    //其实标准规定这里是 undefined behavior
		/*
		以下使用了 placement new，它的形参在正常情况下会传入标准分配函数 void* operator new(std::size_t, void*)，它直接返回未更改的第二实参
		但是如果 operator new 有更多的重载版本，为了避免被其它版本接管，这里先将 p 强转成 void*
		由于这里的 p 是可能有 cv 限定符的可能是任意类型的 T 指针，所以为了去除 cv 限定符需要用 const_cast，为了强转指针类型需要用 static_cast
		*/
		return ::new(const_cast<void*>(static_cast<const volatile void*>(p))) T(forward<Args>(args)...);
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_CONSTRUCT_AT_H_