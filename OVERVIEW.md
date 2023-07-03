# 综述（？

想了想还是决定写一份总结，主要是趁自己还没忘掉太多细节把重要的东西都记下来便于自己以后复习用。如果能帮助到其它新手就更好了

再次强调，由于个人也只是个初学者，经验所限免不了有错误，请自己多加求证

## 1. type_traits

type_traits 是整个 STL 的基石，但实现没什么可说的地方。主要依靠模板特化与偏特化，SFINAE，概念与约束（C++20），也有很多地方（如 is_class）除非编译期开洞否则是不可能做得到的

概念与约束比传统的 SFINAE 更加优雅简洁，而且功能更完备（比如 is_nothrow_convertible 的实现里，noexcept 的检测就不是传统手段能办到的

```cpp
requires {{ declval<void (&)(To) noexcept>()(declval<From>()) } noexcept; }
```

### SFINAE 有两个可注意的点：

(1) allocator_traits&lt;Alloc&gt; 中 Alloc 随着对象实例化就已经被替换完成了，而 SFINAE 是在模板替换时生效的，所以下列函数需要手动添加 class A = Alloc 来促成 SFINAE

```cpp
template<class A = Alloc, class = enable_if_t<allocator_traits_details::has_max_size<const A>::value>>
static constexpr size_type max_size(const A& a) noexcept {
    return a.max_size();
}
```

(2) tuple 的构造函数中有一组 pair 的转换函数（仅在 sizeof...(Types) == 2 时参与重载决议），要求当且仅当下式为 true 时是 explicit 的

```cpp
!is_convertible_v<U1&, tuple_element_t<0, tuple>> || !is_convertible_v<U2&, tuple_element_t<1, tuple>>
```

在 C++20 有 explicit(表达式) 但是不能用在这里，比如 tuple&lt;int&gt; 会提示没有 tuple_element_t<1, tuple>（单参数当然没有了），可能是因为这里的常量表达式求值优先级更高吧。只能用 SFINAE 替代

### 还有一个用于实现 common_type 等地方的一个技巧

对于这种要求为"<u>若某个条件成立就定义 type 为 x，否则若另一个条件成立就定义 type 为 y，否则就定义 type 为 z </u>"的，先如下定义一组空的主模板并确立继承关系，然后再用特化与 SFINAE 在每个条件那检测是否成立并定义 type，这样由于继承关系，只有最优先的条件里的 type 会留存下来

```cpp
template<class T1, class T2> struct common_type_sub_bullet4 {};
template<class T1, class T2> struct common_type_sub_bullet3 : common_type_sub_bullet4<T1, T2> {};
template<class T1, class T2> struct common_type_sub_bullet2 : common_type_sub_bullet3<T1, T2> {};
template<class T1, class T2> struct common_type_sub_bullet1 : common_type_sub_bullet2<T1, T2> {};
```

## 2. tuple

tuple 的实现方式据我了解有两种，第一种是递归继承，效率比较低所以没见谁用。第二种是如下的主流实现（本仓库的实现依赖了 C++17 折叠表达式，主流实现会更复杂一点点）：

主要组件是 index_sequence, tuple_leaf, tuple_impl，整体结构如下：

```cpp
template<size_t I, class T>
struct tuple_leaf;

template<size_t I, class T>
    requires (is_empty_v<T> && !is_final_v<T>)    // 空基类优化版本
struct tuple_leaf<I, T> : T;

template<class Index, class... Types>
struct tuple_impl;

template<size_t... I, class... Types>
struct tuple_impl<index_sequence<I...>, Types...> : tuple_leaf<I, Types> ...;
        
template<class... Types>    // Types... 为空时会用下面的特化，所以主模板中 sizeof...(Types) >= 1 的要求已被抹去
struct tuple {
    using base_type = tuple_impl<index_sequence_for<Types...>, Types...>;
    base_type base;
};

template<>
struct tuple<>;
```

可以发现 tuple_impl 的存在完全是为了帮 tuple 取出 index_sequence<I...> 的形参包，便于后续用 I... 进行的一系列操作

然后 tuple 的每个组分都是通过继承 tuple_leaf 做到的，等后续想用 get 拿出对应的组分时，只需要将 tuple::base 向上转型成对应的 tuple_leaf 即可

### 着重提一下空基类优化

在之后的<u>内含删除器/分配器/比较器</u>的智能指针和容器中都需要空基类优化，主流实现是 compressed_pair，原理跟上面一样都是通过继承空类。而在 C++20 引入了 [[no_unique_address]] 则更加优雅的完成了这个任务，还解决了 final 空类无法被继承的问题

## 3. 智能指针

unique_ptr 没啥可说的，唯一需要注意的是 reset 函数的三步问题

### shared_ptr 与 weak_ptr 的主流实现：

shared_ptr 与 weak_ptr 内部均保有两根指针：

    get() 所返回的指针（也称为存储指针）
    指向控制块的指针

控制块是一个动态分配的对象，其中包含：

    指向被管理对象的指针或被管理对象本身（也称为管理指针）
    删除器（类型擦除）
    分配器（类型擦除）
    占有被管理对象的 shared_ptr 的数量（shared_count）
    涉及被管理对象的 shared_ptr 与 weak_ptr 的数量（shared_weak_count）

存储指针与管理指针不同常见于以下两种场景：

    1、存储指针指向的对象是管理指针指向对象的成员
    2、多态

当 shared_count 归零时，管理指针会被删除器所释放；当 shared_weak_count 归零时，整个控制块会被分配器释放（避免弱悬垂引用）

shared_ptr 内保留的指针是 shared_weak_count*，记录了两个引用计数，控制块继承了 shared_weak_count 并增加了管理指针、删除器与分配器，当 shared_ptr 构造时实际构造的是控制块，然后由 shared_weak_count* 指向它

由于类型擦除，对控制块的某些访问需要通过多态实现。由于外部无法得知控制块的大小，所以控制块的释放由自身完成

感觉 C++23 的 crtp + deducing this 可以取代虚函数多态提高一点效率，不过目前还没法用就暂时作罢

## 4. vector

vector 是动态分配容器中唯一一个可以用裸指针作为迭代器的，但是一般会用带边界检查的 wrap_iter 包一层裸指针作为迭代器

后续的容器也正由于迭代器在中间充当辅助，抹掉了底层实际上并不连续的问题。上层只需要通过迭代器就如同在操纵一整块连续空间

### vector 大概有三个值得提的点（基本也适用于之后的其它容器）

(1) 强异常安全

    构造或尾插入时抛出异常需要在 catch 块中手动析构元素并释放内存

    扩容时只有在 is_nothrow_move_constructible_v<value_type> 为 true 时才会选择移动，否则拷贝

(2) 扩容时注意自引用
    
    为了使 v.push_back(v[0]); 不出错，扩容时需要在新内存上完成所有工作才析构旧内存的对象并释放旧内存

(3) 在中断插入或删除时，元素左右移动要分清是否在未初始化内存上

## 5. list 与 forward_list

这俩的实现细节基本一样。

所有结点都需要有前后指针，但除了 end_node 以外的结点还需要有 value_type，所以结点也是由只含指针的 list_node_base 和继承其并添加 value_type 的 list_node 组成

再就是注意要用 rebind_alloc 即可

## 6. deque

deque 里的 map（每个小数组的控制中心）主流是用类似 vector 的结构实现，此仓库为了简单用的是 list，避免了重分配

同样为了简单起见，与其他容器不同，deque 即使空初始化依然会分配一块堆内存。注意：这使得移动构造行为变为了先本身进行空初始化再与 other 进行 swap

## 7. tree（set 与 map 底层）

结点设计与 list 类似，树的 dummy_head 充当 end_node，该节点（node_base）只需要一根 left 指针（至于为什么是 left，自己写写就知道了），正常结点（node）继承 node_base 并增加 parent, right 指针、value_type 和 color（红黑树独占）

与之前的序列式容器不同，关联式容器在 insert 一个范围的时候，一般不要求做到强异常安全，因为元素插入后就分散开了，很难沿着原路径撤销之前的插入

隔壁 Rust 用 BTree 做底层听说性能很不错

## 8. hashtable（unordered_set 与 unordered_map 底层）

## 9. any