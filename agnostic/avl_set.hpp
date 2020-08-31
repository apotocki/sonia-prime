// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_AVL_SET_HPP
#define AGNOSTIC_AVL_SET_HPP

#pragma once

#include "intrusive/avl.hpp"

namespace agnostic {

namespace avl_set_detail {

template <typename KeyT, typename LessT>
struct element : intrusive::avl_tree_base_hook<>
{
    KeyT key;

    template <typename SKT>
    explicit element(SKT&& k) : key{ std::forward<SKT>(k) } {}

    bool operator< (element const& e) const { return LessT()(key, e.key); }
    template <class OKT> friend bool operator< (element const& l, OKT const& r) { return LessT()(l.key, r); }
    template <class OKT> friend bool operator< (OKT const& l, element const& r) { return LessT()(l, r.key); }
};

template <typename KeyT, typename TreeT>
class iterator
{
    template <typename, typename> friend class iterator;

public:
    using value_type = std::remove_const_t<KeyT>;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;
    using pointer = std::add_pointer_t<value_type>;
    using reference = std::add_lvalue_reference_t<KeyT>;

private:
    using element_t = std::conditional_t<
        std::is_const_v<KeyT>,
        std::add_const_t<typename TreeT::node>,
        typename TreeT::node
    >;

public:
    iterator() : data_{ 1 } {}
    
    explicit iterator(element_t* nd, bool isend = false)
    {
        assert(!(reinterpret_cast<uintptr_t>(nd) & 1));
        data_ = (isend ? 1 : 0) | reinterpret_cast<uintptr_t>(nd);
    }

    template <typename OKeyT, std::enable_if_t<std::is_same_v<std::add_const_t<OKeyT>, KeyT>, int> = 0>
    iterator(iterator<OKeyT, TreeT> const& rhs) : data_{ rhs.data_ } {}

    template <typename OKeyT, std::enable_if_t<std::is_same_v<std::add_const_t<OKeyT>, KeyT>, int> = 0>
    iterator& operator= (iterator<OKeyT, TreeT> const& rhs)
    {
        data_ = rhs.data_;
        return *this;
    }

    bool operator== (iterator const& rhs) const { return data_ == rhs.data_ || (empty() && rhs.empty()); }
    bool operator!= (iterator const& rhs) const { return !this->operator==(rhs); }

    bool empty() const { return !!(data_ & 1); }

    KeyT& operator*() const { return get_node()->key; }
    KeyT* operator->() const { return std::addressof(get_node()->key); }

    iterator& operator++()
    {
        assert (!empty());
        element_t * next = TreeT::next(get_node());
        if (next) {
            set_node(next);
        } else {
            set_empty();
        }
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp(*this);
        this->operator++();
        return tmp;
    }

    iterator& operator--()
    {
        if (empty()) {
            element_t * nd = TreeT::last(TreeT::root(get_node()));
            data_ = 0;
            set_node(nd);
        } else {
            set_node(TreeT::previous(get_node()));
        }
        return *this;
    }

    iterator operator--(int)
    {
        iterator tmp(*this);
        this->operator--();
        return tmp;
    }

    element_t* get_node() const noexcept { return reinterpret_cast<element_t*>(data_ & ~1); }

private:
    void set_node(element_t* nd)
    {
        assert(!(reinterpret_cast<uintptr_t>(nd) & 1));
        data_ = (data_ & 1) | reinterpret_cast<uintptr_t>(nd);
    }

    void set_empty() { data_ |= 1; }

    uintptr_t data_;
};

}

template <
    class KeyT,
    class CompareT = std::less<KeyT>,
    class AllocatorT = std::allocator<KeyT>
>
class avl_set : 
    AllocatorT::template rebind<
        avl_set_detail::element<KeyT, CompareT>
    >::other
{
    using element_t = avl_set_detail::element<KeyT, CompareT>;
    using allocator_t = typename AllocatorT::template rebind<element_t>::other;

    using tree_t = intrusive::avl_tree<
        element_t,
        intrusive::constant_time_size<true>>;

public:
    using key_type = KeyT;
    using value_type = KeyT;
    using size_type = typename tree_t::size_type;
    using difference_type = ptrdiff_t;
    using key_compare = CompareT;
    using value_compare = CompareT;
    using allocator_type = AllocatorT;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = typename std::allocator_traits<AllocatorT>::pointer;
    using const_pointer = typename std::allocator_traits<AllocatorT>::const_pointer;
    using iterator = avl_set_detail::iterator<KeyT, typename tree_t::bstree_t>;
    using const_iterator = avl_set_detail::iterator<const KeyT, typename tree_t::bstree_t>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = element_t;

    avl_set() = default;
    
    avl_set(avl_set && rhs)
        : allocator_t(std::move(static_cast<allocator_t&>(rhs)))
        , impl_(std::move(rhs.impl_))
    {
    
    }

    ~avl_set()
    {
        element_t* nd = impl_.leftmost();
        while (nd) {
            element_t* p = impl_.unlink_without_rebalance(nd);
            this->destroy(nd);
            this->deallocate(nd, 1);
            nd = tree_t::bstree_t::leftmost(p);
        }
    }

    iterator begin() noexcept { return create_iterator(impl_.first()); }
    const_iterator begin() const noexcept { return create_const_iterator(impl_.first()); }
    const_iterator cbegin() const noexcept { return create_const_iterator(impl_.first()); }

    iterator end() noexcept { return iterator{ impl_.root(), true }; }
    const_iterator end() const noexcept { return const_iterator{ impl_.root(), true }; }
    const_iterator cend() const noexcept { return const_iterator{ impl_.root(), true }; }

    bool empty() const noexcept { return impl_.empty(); }

    size_type size() const noexcept { return impl_.size(); }

    template <typename KT>
    const_iterator find(KT const& k) const
    {
        return create_const_iterator(tree_t::bstree_t::find(impl_.root(), k, typename tree_t::compare_t()));
    }

    template <typename KT>
    iterator find(KT const& k)
    {
        return create_iterator(tree_t::bstree_t::find(impl_.root(), k, typename tree_t::compare_t()));
    }

    template <typename T>
    std::pair<iterator, bool> insert(T && value)
    {
        element_t *lb = tree_t::bstree_t::lower_bound(impl_.root(), std::forward<T>(value), typename tree_t::compare_t());
        if (lb && !typename tree_t::compare_t{}(lb->key, value)) {
            return {iterator{lb}, false};
        }
        element_t* e = this->allocate(1);
        try {
            this->construct(e, std::forward<T>(value));
            try {
                return { iterator{impl_.insert_before(lb, *e)}, true};
            } catch (...) {
                this->destroy(e);
                throw;
            }
        } catch (...) {
            this->deallocate(e, 1);
            throw;
        }
    }

    iterator erase(const_iterator pos)
    {
        return do_erase(const_cast<element_t*>(pos.get_node()));
    }

    iterator erase(iterator pos)
    {
        return do_erase(pos.get_node());
    }

    template <typename T>
    size_type erase(T const& key)
    {
        element_t* e = tree_t::bstree_t::find(impl_.root(), key, typename tree_t::compare_t());
        if (e) {
            do_erase(e);
            return 1;
        }
        return 0;
    }

    void check_structure() { impl_.check_structure(); }

private:
    inline const_iterator create_const_iterator(element_t * e) const { return const_iterator{e ? e : impl_.root(), !e}; }
    inline iterator create_iterator(element_t* e) const { return iterator{ e ? e : impl_.root(), !e }; }

    iterator do_erase(element_t* n2r)
    {
        element_t* n = impl_.remove_node(*n2r);
        this->destroy(n2r);
        this->deallocate(n2r, 1);
        return create_iterator(n);
    }

    tree_t impl_;
};

}

#endif // AGNOSTIC_AVL_SET_HPP
