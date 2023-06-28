#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MAP_H_

#include <ciel/bst.h>

namespace ciel {

    // TODO: 红黑树, operator<=>, node_type, insert_return_type, extract, merge

    template<class Key, class T, class Compare = ciel::less<Key>, class Allocator = ciel::allocator<ciel::pair<const Key, T>>>
    class map {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = ciel::pair<const Key, T>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using key_compare = Compare;
        using allocator_type = Allocator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = typename ciel::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename ciel::allocator_traits<allocator_type>::const_pointer;

    private:
        class value_compare {
        protected:
            Compare comp;

        public:
            value_compare(const Compare& c = Compare()) : comp(c) {}

            bool operator()(const value_type& lhs, const value_type& rhs) const {
                return comp(lhs.first, rhs.first);
            }

            bool operator()(const key_type& lhs, const value_type& rhs) const {
                return comp(lhs, rhs.first);
            }

            bool operator()(const value_type& lhs, const key_type& rhs) const {
                return comp(lhs.first, rhs);
            }

        };  // class value_compare

        using tree_type = ciel::bst<value_type, value_compare, allocator_type>;
        using alloc_traits = ciel::allocator_traits<allocator_type>;

    public:
        using iterator = typename tree_type::iterator;
        using const_iterator = typename tree_type::const_iterator;

        using reverse_iterator = ciel::reverse_iterator<iterator>;
        using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

    private:
        tree_type tree;

    public:
        map() : tree() {}

        explicit map(const value_compare& c, const allocator_type& alloc = allocator_type()) : tree(c, alloc) {}

        explicit map(const Allocator& alloc) : tree(alloc) {}

        template<ciel::legacy_input_iterator InputIt>
        map(InputIt first, InputIt last, const value_compare& c = value_compare(), const allocator_type& alloc = allocator_type()) : tree(ciel::false_type{}, first, last, c, alloc) {}

        template<ciel::legacy_input_iterator InputIt>
        map(InputIt first, InputIt last, const allocator_type& alloc) : map(first, last, value_compare(), alloc) {}

        map(const map& other) : tree(other.tree) {}

        map(const map& other, const allocator_type& alloc) : tree(other.tree, alloc) {}

        map(map&& other) noexcept : tree(ciel::move(other.tree)) {}

        map(map&& other, const allocator_type& alloc) : tree(ciel::move(other.tree), alloc) {}

        map(std::initializer_list<value_type> init, const value_compare& c = value_compare(), const allocator_type& alloc = allocator_type()) : map(init.begin(), init.end(), c, alloc) {}

        map(std::initializer_list<value_type> init, const allocator_type& alloc) : map(init, value_compare(), alloc) {}

        ~map() = default;

        map& operator=(const map& other) = default;

        map& operator=(map&& other) noexcept(alloc_traits::is_always_equal::value && ciel::is_nothrow_move_assignable_v<value_compare>) = default;

        map& operator=(std::initializer_list<value_type> ilist) {
            clear();
            tree.range_insert_unique(ilist.begin(), ilist.end());
            return *this;
        }

        allocator_type get_allocator() const noexcept {
            return tree.get_allocator();
        }

        T& at(const Key& key) {
            auto pos = tree.find(key);
            if (pos == end()) {
                throw std::out_of_range("ciel::map 成员函数 at 越界");
            }
            return pos->second;
        }

        const T& at(const Key& key) const {
            auto pos = tree.find(key);
            if (pos == end()) {
                throw std::out_of_range("ciel::map 成员函数 at 越界");
            }
            return pos->second;
        }

        T& operator[](const Key& key) {
            return try_emplace(key).first->second;
        }

        T& operator[](Key&& key) {
            return try_emplace(ciel::move(key)).first->second;
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

        template<class P>
        ciel::pair<iterator, bool> insert(P&& value) requires ciel::is_constructible_v<value_type, P&&> {
            return emplace(ciel::move(value));
        }

        ciel::pair<iterator, bool> insert(value_type&& value) {
            return emplace(ciel::move(value));
        }

        iterator insert(const_iterator pos, const value_type& value) {
            return emplace_hint(pos, value);
        }

        template<class P>
        iterator insert(const_iterator pos, P&& value) requires ciel::is_constructible_v<value_type, P&&> {
            return emplace_hint(pos, ciel::move(value));
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

        template<class M>
        ciel::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) requires ciel::is_assignable_v<mapped_type&, M&&> {
            auto pos = find(k);
            if (pos == end()) {
                return emplace(k, ciel::move(obj));
            } else {
                pos->second = ciel::move(obj);
                return {pos, false};
            }
        }

        template<class M>
        ciel::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj) requires ciel::is_assignable_v<mapped_type&, M&&> {
            auto pos = find(k);
            if (pos == end()) {
                return emplace(ciel::move(k), ciel::move(obj));
            } else {
                pos->second = ciel::move(obj);
                return {pos, false};
            }
        }

        template<class M>
        iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj) requires ciel::is_assignable_v<mapped_type&, M&&> {
            auto pos = find(k);
            if (pos == end()) {
                return emplace_hint(hint, k, ciel::move(obj));
            } else {
                pos->second = ciel::move(obj);
                return pos;
            }
        }

        template<class M>
        iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj) requires ciel::is_assignable_v<mapped_type&, M&&> {
            auto pos = find(k);
            if (pos == end()) {
                return emplace_hint(hint, ciel::move(k), ciel::move(obj));
            } else {
                pos->second = ciel::move(obj);
                return pos;
            }
        }

        template<class... Args>
        ciel::pair<iterator, bool> emplace(Args&& ... args) {
            return tree.emplace_unique(ciel::forward<Args>(args)...);
        }

        template<class... Args>
        iterator emplace_hint(const_iterator hint, Args&& ... args) {
            return tree.emplace_unique_hint(hint, ciel::forward<Args>(args)...);
        }

        template<class... Args>
        ciel::pair<iterator, bool> try_emplace(const key_type& k, Args&& ... args) {
            auto pos = find(k);
            if (pos == end()) {
                return emplace(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(ciel::forward<Args>(args)...));
            } else {
                return {pos, false};
            }
        }

        template<class... Args>
        ciel::pair<iterator, bool> try_emplace(key_type&& k, Args&& ... args) {
            auto pos = find(k);
            if (pos == end()) {
                return emplace(std::piecewise_construct, std::forward_as_tuple(ciel::move(k)), std::forward_as_tuple(ciel::forward<Args>(args)...));
            } else {
                return {pos, false};
            }
        }

        template<class... Args>
        iterator try_emplace(const_iterator hint, const key_type& k, Args&& ... args) {
            auto pos = find(k);
            if (pos == end()) {
                return emplace_hint(hint, std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(ciel::forward<Args>(args)...));
            } else {
                return {pos, false};
            }
        }

        template<class... Args>
        iterator try_emplace(const_iterator hint, key_type&& k, Args&& ... args) {
            auto pos = find(k);
            if (pos == end()) {
                return emplace_hint(hint, std::piecewise_construct, std::forward_as_tuple(ciel::move(k)), std::forward_as_tuple(ciel::forward<Args>(args)...));
            } else {
                return {pos, false};
            }
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

        void swap(map& other) noexcept(alloc_traits::is_always_equal::value && ciel::is_nothrow_swappable_v<value_compare>) {
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
            return key_compare();
        }

        value_compare value_comp() const {
            return tree.value_comp();
        }

    };  // class map

    template<class Key, class T, class Compare, class Alloc>
    bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class Key, class T, class Compare, class Alloc>
    void swap(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }

    template<class Key, class T, class Compare, class Alloc, class Pred>
    typename map<Key, T, Compare, Alloc>::size_type erase_if(map<Key, T, Compare, Alloc>& c, Pred pred) {
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

    template<class InputIt, class Comp = ciel::less<ciel::remove_const_t<typename ciel::iterator_traits<InputIt>::value_type::first_type>>, class Alloc = ciel::allocator<ciel::pair<ciel::add_const_t<typename ciel::iterator_traits<InputIt>::value_type::first_type>, typename ciel::iterator_traits<InputIt>::value_type::second_type>>>
    map(InputIt, InputIt, Comp = Comp(), Alloc = Alloc()) -> map<ciel::remove_const_t<typename ciel::iterator_traits<InputIt>::value_type::first_type>, typename ciel::iterator_traits<InputIt>::value_type::second_type, Comp, Alloc>;

    template<class Key, class T, class Comp = ciel::less<Key>, class Alloc = ciel::allocator<ciel::pair<const Key, T>>>
    map(std::initializer_list<ciel::pair<const Key, T>>, Comp = Comp(), Alloc = Alloc()) -> map<Key, T, Comp, Alloc>;

    template<class InputIt, class Alloc>
    map(InputIt, InputIt, Alloc) -> map<ciel::remove_const_t<typename ciel::iterator_traits<InputIt>::value_type::first_type>, typename ciel::iterator_traits<InputIt>::value_type::second_type, ciel::less<ciel::remove_const_t<typename ciel::iterator_traits<InputIt>::value_type::first_type>>, Alloc>;

    template<class Key, class T, class Allocator>
    map(std::initializer_list<ciel::pair<const Key, T>>, Allocator) -> map<Key, T, ciel::less<Key>, Allocator>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MAP_H_