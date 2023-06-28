#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_BST_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_BST_H_

#include <ciel/memory.h>
#include <ciel/iterator.h>
#include <ciel/algorithm.h>

namespace ciel {

    // 非平衡二叉搜索树，仅为熟悉相关实现而设。TODO: 后面会改为红黑树

    // 结点设计与 list 类似，树的 dummy_head 充当 end 结点，该节点（node_base）只需要一根 left 指针，正常结点（node）继承 node_base 并增加 parent, right 指针与 value 数据成员（后续红黑树结点还需要一位 char 来作标识）
    // dummy_head 通过 left 指针与真正的头结点相连的原因是可使得迭代器的前进与后退操作变得通用：
    //      前进操作：如果 right 不为空，则从 right 开始走到最左下；否则一直回到 parent，直到自身是 parent->left，这时 parent 即为目标
    //          所以树中最大结点前进时，会一直回到 parent 直到头结点，然后由于头结点是 dummy_head->left，目标则刚好为 dummy_head
    //      后退操作：如果 left 不为空，则从 left 开始走到最右下；否则一直回到 parent，直到自身不是 parent->left，这时 parent 即为目标
    //          所以从 end 结点也就是 dummy_head 后退时，先到 left 然后一直到达最右下，刚好就是树中最大结点

    // 注意：与之前的序列式容器不同，关联式容器在 insert 一个范围的时候，一般不要求做到强异常安全，因为元素插入后就分散开了，很难沿着原路径撤销之前的插入

    // TODO: 不打算写 multi 版的容器了，本来就没啥用，还全是重复代码。不过接口都写了。（主要是懒得写测试= =

    struct bst_node_base {
        bst_node_base* left;

        bst_node_base(bst_node_base* l) : left(l) {}

    };  // struct bst_node_base

    template<class T>
    struct bst_node : bst_node_base {
        bst_node_base* right;
        bst_node_base* parent;
        T value;

        template<class... Args>
        bst_node(bst_node_base* l, bst_node_base* r, bst_node_base* p, Args... args) : bst_node_base(l), right(r), parent(p), value(ciel::forward<Args>(args)...) {}

    };  // struct bst_node

    template<class T, class Pointer, class Reference>
    class bst_iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = Pointer;
        using reference = Reference;
        using iterator_category = ciel::bidirectional_iterator_tag;
        using iterator_concept = ciel::bidirectional_iterator_tag;

    private:
        using base_node_type = bst_node_base;
        using node_type = bst_node<value_type>;

        base_node_type* it;

    public:
        [[nodiscard]] bool tree_is_left_child() const noexcept {
            return static_cast<node_type*>(it)->parent->left == it;
        }

        void goto_tree_min() noexcept {
            while (it->left) {
                it = it->left;
            }
        }

        void goto_tree_max() noexcept {
            while (right()) {
                it = right();
            }
        }

        bst_iterator() noexcept : it(nullptr) {}

        bst_iterator(const base_node_type* p) noexcept : it(const_cast<base_node_type*>(p)) {}    // const_cast 是为了 const_iterator -> iterator

        bst_iterator(const bst_iterator& other) noexcept : it(other.base()) {}

        template<class P, class R>
        bst_iterator(const bst_iterator<T, P, R>& other) noexcept : it(const_cast<base_node_type*>(other.base())) {}

        bst_iterator next() const noexcept {
            bst_iterator res(it);
            ++res;
            return res;
        }

        bst_iterator prev() const noexcept {
            bst_iterator res(it);
            --res;
            return res;
        }

        reference operator*() const noexcept {
            return (static_cast<node_type*>(it))->value;
        }

        pointer operator->() const noexcept {
            return &static_cast<node_type*>(it)->value;
        }

        bst_iterator& operator++() noexcept {
            if (right()) {
                it = right();
                goto_tree_min();
            } else {
                while (!tree_is_left_child()) {
                    it = parent();
                }
                it = parent();
            }
            return *this;
        }

        bst_iterator operator++(int) noexcept {
            bst_iterator res(it);
            ++*this;
            return res;
        }

        bst_iterator& operator--() noexcept {
            if (left()) {
                it = left();
                goto_tree_max();
            } else {
                while (tree_is_left_child()) {
                    it = parent();
                }
                it = parent();
            }
            return *this;
        }

        bst_iterator operator--(int) noexcept {
            bst_iterator res(it);
            --*this;
            return res;
        }

        [[nodiscard]] base_node_type* base() const noexcept {
            return it;
        }

        [[nodiscard]] base_node_type* left() const noexcept {
            return it->left;
        }

        [[nodiscard]] base_node_type* right() const noexcept {
            return static_cast<node_type*>(it)->right;
        }

        [[nodiscard]] base_node_type* parent() const noexcept {
            return static_cast<node_type*>(it)->parent;
        }

        operator bool() {
            return it != nullptr;
        }

    };  // class bst_iterator

    template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2>
    bool operator==(const bst_iterator<T, Pointer1, Reference1>& lhs, const bst_iterator<T, Pointer2, Reference2>& rhs) {
        return lhs.base() == rhs.base();
    }

    template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2>
    bool operator!=(const bst_iterator<T, Pointer1, Reference1>& lhs, const bst_iterator<T, Pointer2, Reference2>& rhs) {
        return !(lhs == rhs);
    }

    template<class T, class Compare, class Allocator>
    class bst {
    public:
        using value_type = T;
        using value_compare = Compare;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using allocator_type = Allocator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = typename ciel::allocator_traits<Allocator>::pointer;
        using const_pointer = typename ciel::allocator_traits<Allocator>::const_pointer;
        using iterator = bst_iterator<value_type, pointer, reference>;
        using const_iterator = bst_iterator<value_type, const_pointer, const_reference>;
        using reverse_iterator = ciel::reverse_iterator<iterator>;
        using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

    private:
        using base_node_type = bst_node_base;
        using node_type = bst_node<value_type>;

        using alloc_traits = ciel::allocator_traits<allocator_type>;
        using node_allocator = typename alloc_traits::template rebind_alloc<node_type>;
        using node_alloc_traits = typename alloc_traits::template rebind_traits<node_type>;

        base_node_type* start;
        base_node_type end_node;
        size_type s;
        [[no_unique_address]] node_allocator allocator;
        [[no_unique_address]] value_compare comp;

        // 递归深度为 H，仅用于 clear
        void destroy_and_deallocate_root(iterator root) noexcept {
            if (root) {
                destroy_and_deallocate_root(root.left());
                destroy_and_deallocate_root(root.right());
                node_alloc_traits::destroy(allocator, static_cast<node_type*>(root.base()));
                node_alloc_traits::deallocate(allocator, static_cast<node_type*>(root.base()), 1);
            }
        }

        void destroy_and_deallocate_node(iterator node) noexcept {
            if (!node.parent()) {   // 结点造出来后因为无法插入重复元素会由此直接删除
                goto LABEL;
            }
            if (node == begin()) {
                start = node.next().base();
            }
            if (node.left()) {
                if (node.tree_is_left_child()) {
                    node.parent()->left = node.left();
                } else {
                    static_cast<node_type*>(node.parent())->right = node.left();
                }
                static_cast<node_type*>(node.left())->parent = node.parent();
                if (node.right()) {
                    iterator tmp = node.left();
                    tmp.goto_tree_max();
                    static_cast<node_type*>(tmp.base())->right = node.right();
                    static_cast<node_type*>(node.right())->parent = tmp.base();
                }
            } else if (node.right()) {
                if (node.tree_is_left_child()) {
                    node.parent()->left = node.right();
                } else {
                    static_cast<node_type*>(node.parent())->right = node.right();
                }
                static_cast<node_type*>(node.right())->parent = node.parent();
            }
            LABEL:
            node_alloc_traits::destroy(allocator, static_cast<node_type*>(node.base()));
            node_alloc_traits::deallocate(allocator, static_cast<node_type*>(node.base()), 1);
            --s;
        }

        template<class Key>
        bool is_right_insert_unique_place(iterator pos, const Key& key) const noexcept {
            if ((pos == end() || comp(key, *pos)) && (pos == begin() || comp(*pos.prev(), key))) {
                return true;
            } else {
                return false;
            }
        }

        template<class Key>
        bool is_right_insert_equal_place(iterator pos, const Key& key) const noexcept {
            if ((pos == end() || !comp(*pos, key)) && (pos == begin() || !comp(key, *pos.prev()))) {
                return true;
            } else {
                return false;
            }
        }

        template<class... Args>
        node_type* allocate_and_construct_node(Args&& ... args) {
            node_type* new_node = node_alloc_traits::allocate(allocator, 1);
            try {
                node_alloc_traits::construct(allocator, new_node, nullptr, nullptr, nullptr, ciel::forward<Args>(args)...);
            } catch (...) {
                node_alloc_traits::deallocate(allocator, new_node, 1);
                throw;
            }
            ++s;
            return new_node;
        }

        iterator insert_node_at(iterator pos, node_type* new_node) {
            if (pos.left()) {
                iterator l = pos.left();
                l.goto_tree_max();
                static_cast<node_type*>(l.base())->right = new_node;
                new_node->parent = l.base();
                return l.right();
            } else {
                pos.base()->left = new_node;
                new_node->parent = pos.base();
                if (pos.base() == start) {
                    start = new_node;
                }
                return pos.left();
            }
        }

    public:
        bst() : start(&end_node), end_node(nullptr), s(0), allocator(), comp() {}

        explicit bst(const Compare& c, const Allocator& alloc = Allocator()) : start(&end_node), end_node(nullptr), s(0), allocator(alloc), comp(c) {}

        explicit bst(const Allocator& alloc) : start(&end_node), end_node(nullptr), s(0), allocator(alloc), comp() {}

        template<class InputIt>
        bst(ciel::true_type, InputIt first, InputIt last, const Compare& c = Compare(), const Allocator& alloc = Allocator()) : start(&end_node), end_node(nullptr), s(0), allocator(alloc), comp(c) {
            range_insert_equal(first, last);
        }

        template<class InputIt>
        bst(ciel::false_type, InputIt first, InputIt last, const Compare& c = Compare(), const Allocator& alloc = Allocator()) : start(&end_node), end_node(nullptr), s(0), allocator(alloc), comp(c) {
            range_insert_unique(first, last);
        }

        bst(const bst& other) : start(&end_node), end_node(nullptr), s(0), allocator(node_alloc_traits::select_on_container_copy_construction(other.get_allocator())), comp() {
            range_insert_equal(other.begin(), other.end());
        }

        bst(const bst& other, const Allocator& alloc) : start(&end_node), end_node(nullptr), s(0), allocator(alloc), comp() {
            range_insert_equal(other.begin(), other.end());
        }

        bst(bst&& other) noexcept : start(other.s ? other.start : &end_node), end_node(other.end_node), s(other.s), allocator(ciel::move(other.allocator)), comp(ciel::move(other.comp)) {
            if (end_node.left) {
                static_cast<node_type*>(end_node.left)->parent = &end_node;
            }
            other.start = &other.end_node;
            other.end_node.left = nullptr;
            other.s = 0;
        }

        bst(bst&& other, const Allocator& alloc) : start(other.s ? other.start : &end_node), end_node(other.end_node), s(other.s), allocator(alloc), comp(ciel::move(other.comp)) {
            if (end_node.left) {
                static_cast<node_type*>(end_node.left)->parent = &end_node;
            }
            other.start = &other.end_node;
            other.end_node.left = nullptr;
            other.s = 0;
        }

        ~bst() {
            clear();
        }

        // 若 alloc_traits::propagate_on_container_copy_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器
        // 若 *this 的分配器在赋值后将与其旧值比较不相等，则用旧分配器解分配内存，然后在复制元素前用新分配器分配内存
        // 否则，在可行时可能复用 *this 所拥有的内存
        bst& operator=(const bst& other) {
            if (this == ciel::addressof(other)) {
                return *this;
            }
            if (alloc_traits::propagate_on_container_copy_assignment::value) {
                if (allocator != other.allocator) {
                    {
                        bst tmp(other.allocator);
                        swap(tmp);
                    }
                    range_insert_equal(other.begin(), other.end());
                    return *this;
                }
                allocator = other.allocator;
            }
            clear();
            range_insert_equal(other.begin(), other.end());
            return *this;
        }

        // 若 alloc_traits::propagate_on_container_move_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器
        // 若它为 false 且 *this 与 other 的分配器不比较相等，则 *this 不能接管 other 所拥有的内存的所有权且必须单独地移动赋值每个元素，并用其自身的分配器按需分配额外内存
        bst& operator=(bst&& other) noexcept {
            if (this == ciel::addressof(other)) {
                return *this;
            }
            if (!alloc_traits::propagate_on_container_move_assignment::value && allocator != other.allocator) {
                clear();
                range_insert_equal(other.begin(), other.end());
                return *this;
            }
            if (alloc_traits::propagate_on_container_move_assignment::value) {
                allocator = other.allocator;
            }
            clear();
            start = other.s ? other.start : &end_node;
            end_node = other.end_node;
            if (end_node.left) {
                static_cast<node_type*>(end_node.left)->parent = &end_node;
            }
            s = other.s;
            other.start = &other.end_node;
            other.end_node.left = nullptr;
            other.s = 0;
            return *this;
        }

        allocator_type get_allocator() const noexcept {
            return allocator;
        }

        iterator begin() noexcept {
            return iterator(start);
        }

        const_iterator begin() const noexcept {
            return const_iterator(start);
        }

        iterator end() noexcept {
            return iterator(&end_node);
        }

        const_iterator end() const noexcept {
            return const_iterator(&end_node);
        }

        [[nodiscard]] bool empty() const noexcept {
            return size() == 0;
        }

        [[nodiscard]] size_type size() const noexcept {
            return s;
        }

        [[nodiscard]] size_type max_size() const noexcept {
            return ciel::numeric_limits<difference_type>::max();
        }

        void clear() noexcept {
            destroy_and_deallocate_root(end().left());
            end_node.left = nullptr;
            start = &end_node;
            s = 0;
        }

        // 当 [first, last) 为有序范围时，时间复杂度应由乱序的 N * H 变为 H + N
        template<ciel::legacy_input_iterator InputIt>
        void range_insert_unique(InputIt first, InputIt last) {
            if (ciel::is_sorted(first, last, comp)) {
                iterator pos = lower_bound(*first);
                while (first != last) {
                    while (!is_right_insert_equal_place(pos, *first)) {
                        ++pos;
                    }
                    if (is_right_insert_unique_place(pos, *first)) {
                        insert_node_at(pos, allocate_and_construct_node(*first));
                    }
                    ++first;
                }
            } else {
                while (first != last) {
                    iterator pos = lower_bound(*first);
                    if (is_right_insert_unique_place(pos, *first)) {
                        insert_node_at(pos, allocate_and_construct_node(*first));
                    }
                    ++first;
                }
            }
        }

        template<ciel::legacy_input_iterator InputIt>
        void range_insert_equal(InputIt first, InputIt last) {
            if (ciel::is_sorted(first, last, comp)) {
                iterator pos = lower_bound(*first);
                while (first != last) {
                    while (!is_right_insert_equal_place(pos, *first)) {
                        ++pos;
                    }
                    insert_node_at(pos, allocate_and_construct_node(*first));
                    ++first;
                }
            } else {
                while (first != last) {
                    insert_node_at(lower_bound(*first), allocate_and_construct_node(*first));
                    ++first;
                }
            }
        }

        template<class... Args>
        ciel::pair<iterator, bool> emplace_unique(Args&& ... args) {
            node_type* new_node = allocate_and_construct_node(ciel::forward<Args>(args)...);
            iterator hint = lower_bound(new_node->value);
            if (is_right_insert_unique_place(hint, new_node->value)) {
                return {insert_node_at(hint, new_node), true};
            } else {
                destroy_and_deallocate_node(new_node);
                return {hint, false};
            }
        }

        template<class... Args>
        iterator emplace_equal(Args&& ... args) {
            node_type* new_node = allocate_and_construct_node(ciel::forward<Args>(args)...);
            iterator hint = lower_bound(new_node->value);
            return insert_node_at(hint, new_node);
        }

        template<class... Args>
        iterator emplace_unique_hint(const_iterator hint, Args&& ... args) {
            node_type* new_node = allocate_and_construct_node(ciel::forward<Args>(args)...);
            if (!is_right_insert_equal_place(hint, new_node->value)) {
                hint = lower_bound(new_node->value);
            }
            if (is_right_insert_unique_place(hint, new_node->value)) {
                return insert_node_at(hint, new_node);
            } else {
                destroy_and_deallocate_node(new_node);
                return hint;
            }
        }

        template<class... Args>
        iterator emplace_equal_hint(const_iterator hint, Args&& ... args) {
            node_type* new_node = allocate_and_construct_node(ciel::forward<Args>(args)...);
            if (!is_right_insert_equal_place(hint, new_node->value)) {
                hint = lower_bound(new_node->value);
            }
            return insert_node_at(hint, new_node);
        }

        iterator erase(iterator first, iterator last) {
            while (first != last) {
                destroy_and_deallocate_node(first++);
            }
            return last;
        }

        template<class Key>
        size_type erase(const Key& key) {
            auto er = equal_range(key);
            size_type res = size();
            erase(er.first, er.second);
            return res - size();
        }

        void swap(bst& other) noexcept(alloc_traits::is_always_equal::value && ciel::is_nothrow_swappable_v<value_compare>) {
            auto temp_start = start;
            ciel::swap(end_node, other.end_node);
            if (end_node.left) {
                static_cast<node_type*>(end_node.left)->parent = &end_node;
                start = other.start;
            } else {
                start = &end_node;
            }
            if (other.end_node.left) {
                static_cast<node_type*>(other.end_node.left)->parent = &other.end_node;
                other.start = temp_start;
            } else {
                other.start = &other.end_node;
            }
            ciel::swap(s, other.s);
            ciel::swap(allocator, other.allocator);
            ciel::swap(comp, other.comp);
        }

        // TODO: extract, merge

        template<class Key>
        size_type count(const Key& key) const {
            auto er = equal_range(key);
            return ciel::distance(er.first, er.second);
        }

        template<class Key>
        const_iterator find(const Key& key) const {
            auto lb = lower_bound(key);
            if (lb != end() && !comp(key, *lb)) {
                return lb;
            } else {
                return end();
            }
        }

        template<class Key>
        bool contains(const Key& key) const {
            auto er = equal_range(key);
            return er.first != er.second;
        }

        template<class Key>
        ciel::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
            return {lower_bound(key), upper_bound(key)};
        }

        template<class Key>
        const_iterator lower_bound(const Key& key) const {
            iterator root = end().left(), res = end();
            while (root) {
                if (!comp(*root, key)) {
                    res = root;
                    root = root.left();
                } else {
                    root = root.right();
                }
            }
            return res;
        }

        template<class Key>
        const_iterator upper_bound(const Key& key) const {
            iterator root = end().left(), res = end();
            while (root) {
                if (comp(key, *root)) {
                    res = root;
                    root = root.left();
                } else {
                    root = root.right();
                }
            }
            return res;
        }

        value_compare value_comp() const {
            return comp;
        }

    };  // class bst

    template<class Key, class Compare, class Alloc>
    bool operator==(const bst<Key, Compare, Alloc>& lhs, const bst<Key, Compare, Alloc>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class Key, class Compare, class Alloc>
    void swap(bst<Key, Compare, Alloc>& lhs, bst<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_BST_H_