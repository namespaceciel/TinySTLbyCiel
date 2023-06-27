#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_SET_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_SET_H_

#include <ciel/bst.h>

namespace ciel {

    // TODO: 红黑树, operator<=>, node_type, insert_return_type, extract, merge

    template<class Key, class Compare = ciel::less<Key>, class Allocator = ciel::allocator<Key>>
    class set {
    public:
        using key_type = Key;
        using value_type = Key;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using key_compare = Compare;
        using value_compare = Compare;
        using allocator_type = Allocator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = typename ciel::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename ciel::allocator_traits<allocator_type>::const_pointer;

    private:
        using tree_type = ciel::bst<value_type, value_compare, allocator_type>;
        using alloc_traits = ciel::allocator_traits<allocator_type>;

    public:
        using iterator = typename tree_type::iterator;  // FIXME: set 值不应被修改
        using const_iterator = typename tree_type::const_iterator;

        using reverse_iterator = ciel::reverse_iterator<iterator>;
        using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

    private:
        tree_type tree;

    public:
        set() : tree() {}

        explicit set(const value_compare& c, const allocator_type& alloc = allocator_type()) : tree(c, alloc) {}

        explicit set(const allocator_type& alloc) : tree(alloc) {}

        template<ciel::legacy_input_iterator InputIt>
        set(InputIt first, InputIt last, const value_compare& c = Compare(), const allocator_type& alloc = allocator_type()) : tree(ciel::false_type{}, first, last, c, alloc) {}

        template<ciel::legacy_input_iterator InputIt>
        set(InputIt first, InputIt last, const allocator_type& alloc): set(first, last, value_compare(), alloc) {}

        set(const set& other) : tree(other.tree) {}

        set(const set& other, const allocator_type& alloc) : tree(other.tree, alloc) {}

        set(set&& other) noexcept : tree(ciel::move(other.tree)) {}

        set(set&& other, const allocator_type& alloc) : tree(ciel::move(other.tree), alloc) {}

        set(std::initializer_list<value_type> init, const value_compare& c = value_compare(), const allocator_type& alloc = allocator_type()) : set(init.begin(), init.end(), c, alloc) {}

        set(std::initializer_list<value_type> init, const allocator_type& alloc) : set(init, value_compare(), alloc) {}

        ~set() = default;

        set& operator=(const set& other) = default;

        set& operator=(set&& other) noexcept(alloc_traits::is_always_equal::value && ciel::is_nothrow_move_assignable_v<value_compare>) = default;

        set& operator=(std::initializer_list<value_type> ilist) {
            clear();
            tree.range_insert_unique(ilist.begin(), ilist.end());
            return *this;
        }

        allocator_type get_allocator() const noexcept {
            return tree.get_allocator();
        }

        iterator begin() noexcept {
            return tree.begin();
        }

        const_iterator begin() const noexcept {
            return tree.begin();
        }

        const_iterator cbegin() const noexcept {
            return begin();
        }

        iterator end() noexcept {
            return tree.end();
        }

        const_iterator end() const noexcept {
            return tree.end();
        }

        const_iterator cend() const noexcept {
            return end();
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator crend() const noexcept {
            return rend();
        }

        [[nodiscard]] bool empty() const noexcept {
            return tree.empty();
        }

        [[nodiscard]] size_type size() const noexcept {
            return tree.size();
        }

        [[nodiscard]] size_type max_size() const noexcept {
            return tree.max_size();
        }

        void clear() noexcept {
            tree.clear();
        }

        ciel::pair<iterator, bool> insert(const value_type& value) {
            return emplace(value);
        }

        ciel::pair<iterator, bool> insert(value_type&& value) {
            return emplace(ciel::move(value));
        }

        iterator insert(const_iterator pos, const value_type& value) {
            return emplace_hint(pos, value);
        }

        iterator insert(const_iterator pos, value_type&& value) {
            return emplace_hint(pos, ciel::move(value));
        }

        template<ciel::legacy_input_iterator InputIt>
        void insert(InputIt first, InputIt last) {
            tree.range_insert_unique(first, last);
        }

        void insert(std::initializer_list<value_type> ilist) {
            insert(ilist.begin(), ilist.end());
        }

        template<class... Args>
        ciel::pair<iterator, bool> emplace(Args&& ... args) {
            return tree.emplace_unique(ciel::forward<Args>(args)...);
        }

        template<class... Args>
        iterator emplace_hint(const_iterator hint, Args&& ... args) {
            return tree.emplace_unique_hint(hint, ciel::forward<Args>(args)...);
        }

        iterator erase(const_iterator pos) {
            return erase(pos, pos.next());
        }

        iterator erase(const_iterator first, const_iterator last) {
            return tree.erase(first, last);
        }

        size_type erase(const Key& key) {
            return tree.erase(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        size_type erase(K&& x) {
            return tree.erase(x);
        }

        void swap(set& other) noexcept(alloc_traits::is_always_equal::value && ciel::is_nothrow_swappable_v<value_compare>) {
            tree.swap(other.tree);
        }

        size_type count(const Key& key) const {
            return tree.count(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        size_type count(const K& x) const {
            return tree.count(x);
        }

        iterator find(const Key& key) {
            return tree.find(key);
        }

        const_iterator find(const Key& key) const {
            return tree.find(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        iterator find(const K& x) {
            return tree.find(x);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        const_iterator find(const K& x) const {
            return tree.find(x);
        }

        bool contains(const Key& key) const {
            return tree.contains(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        bool contains(const K& x) const {
            return tree.contains(x);
        }

        ciel::pair<iterator, iterator> equal_range(const Key& key) {
            return tree.equal_range(key);
        }

        ciel::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
            return tree.equal_range(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        ciel::pair<iterator, iterator> equal_range(const K& x) {
            return tree.equal_range(x);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        ciel::pair<const_iterator, const_iterator> equal_range(const K& x) const {
            return tree.equal_range(x);
        }

        iterator lower_bound(const Key& key) {
            return tree.lower_bound(key);
        }

        const_iterator lower_bound(const Key& key) const {
            return tree.lower_bound(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        iterator lower_bound(const K& x) {
            return tree.lower_bound(x);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        const_iterator lower_bound(const K& x) const {
            return tree.lower_bound(x);
        }

        iterator upper_bound(const Key& key) {
            return tree.upper_bound(key);
        }

        const_iterator upper_bound(const Key& key) const {
            return tree.upper_bound(key);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        iterator upper_bound(const K& x) {
            return tree.upper_bound(x);
        }

        template<class K>
            requires requires { typename value_compare::is_transparent; }
        const_iterator upper_bound(const K& x) const {
            return tree.upper_bound(x);
        }

        key_compare key_comp() const {
            return value_comp();
        }

        value_compare value_comp() const {
            return tree.value_comp();
        }

    };  // class set

    template<class Key, class Compare, class Alloc>
    bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class Key, class Compare, class Alloc>
    void swap(set<Key, Compare, Alloc>& lhs, set<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }

    template<class Key, class Compare, class Alloc, class Pred>
    typename set<Key, Compare, Alloc>::size_type erase_if(set<Key, Compare, Alloc>& c, Pred pred) {
        auto old_size = c.size();
        for (auto i = c.begin(), last = c.end(); i != last;) {
            if (pred(*i)) {
                i = c.erase(i);
            } else {
                ++i;
            }
        }
        return old_size - c.size();
    }

    template<ciel::legacy_input_iterator InputIt, class Comp = ciel::less<typename ciel::iterator_traits<InputIt>::value_type>, class Alloc = ciel::allocator<typename ciel::iterator_traits<InputIt>::value_type>>
    set(InputIt, InputIt, Comp = Comp(), Alloc = Alloc()) -> set<typename ciel::iterator_traits<InputIt>::value_type, Comp, Alloc>;

    template<class Key, class Comp = ciel::less<Key>, class Alloc = ciel::allocator<Key>>
    set(std::initializer_list<Key>, Comp = Comp(), Alloc = Alloc()) -> set<Key, Comp, Alloc>;

    template<ciel::legacy_input_iterator InputIt, class Alloc>
    set(InputIt, InputIt, Alloc) -> set<typename ciel::iterator_traits<InputIt>::value_type, ciel::less<typename ciel::iterator_traits<InputIt>::value_type>, Alloc>;

    template<class Key, class Alloc>
    set(std::initializer_list<Key>, Alloc) -> set<Key, ciel::less<Key>, Alloc>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_SET_H_