// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_INTRUSIVE_AVL_HPP
#define AGNOSTIC_INTRUSIVE_AVL_HPP

#pragma once

#ifndef DO_NOT_USE_AGNOSTIC_ADDRESSOF
#   include "../std/memory/addressof.hpp"
#endif

#include "detail/intrusive.hpp"
#include "bstree.hpp"

namespace agnostic::intrusive {

namespace avl_detail { template <class T, class DescriptorT> class hook; }

template <typename ... OptionsT>
class avl_tree_base_hook
{
    template <class, class> friend class avl_detail::hook;

    void* left;
    void* right;
    uintptr_t root;
};

template <typename ... OptionsT>
class avl_tree_member_hook
{
    template <class, class> friend class avl_detail::hook;

    void* left;
    void* right;
    uintptr_t root;
};

namespace avl_detail {

using namespace agnostic::intrusive::detail;

template <class T, class DescriptorT> class hook;

template <class T> class hook<T, void> // base_hook
{
public:
    static inline T*& left(T* nd) noexcept { return reinterpret_cast<T*&>(nd->left); }
    static inline T*& right(T* nd) noexcept { return reinterpret_cast<T*&>(nd->right); }
    static inline T* get_root(T* nd) noexcept { return reinterpret_cast<T*>(nd->root & ~3); }
    static inline void set_root(T* nd, T * val) noexcept
    {
        assert (!(reinterpret_cast<uintptr_t>(val) & 3));
        nd->root = (nd->root & 3) | reinterpret_cast<uintptr_t>(val);
    }
    static inline int get_balance(T* nd) noexcept { return (nd->root & 3) - 1; }
    static inline void set_balance(T* nd, int val) noexcept { assert(val >= -1 && val <= 1); nd->root = (nd->root & ~3) | (val + 1); }

    static inline int dec_balance(T* nd) noexcept
    {
        int bp1 = (nd->root & 3); // the balance + 1
        if (bp1 == 0) return -2; else { --nd->root; return bp1 - 2; }
    }

    static inline int inc_balance(T* nd) noexcept
    {
        int bp1 = (nd->root & 3); // the balance + 1
        if (bp1 == 2) return 2; else { ++nd->root; return bp1; }
    }
};

template <class T, class HookT, HookT T::* PtrToMemberV> class hook<T, member_hook<T, HookT, PtrToMemberV>> // member_hook
{
public:
    static inline T*& left(T* nd) noexcept { return reinterpret_cast<T*&>((nd->*PtrToMemberV).left); }
    static inline T*& right(T* nd) noexcept { return reinterpret_cast<T*&>((nd->*PtrToMemberV).right); }
    static inline T* get_root(T* nd) noexcept { return reinterpret_cast<T*>((nd->*PtrToMemberV).root & ~3); }
    static inline void set_root(T* nd, T* val) noexcept
    {
        assert(!(reinterpret_cast<uintptr_t>(val) & 3));
        (nd->*PtrToMemberV).root = ((nd->*PtrToMemberV).root & 3) | reinterpret_cast<uintptr_t>(val);
    }
    static inline int get_balance(T* nd) noexcept { return ((nd->*PtrToMemberV).root & 3) - 1; }
    static inline void set_balance(T* nd, int val) noexcept { assert(val >= -1 && val <= 1); (nd->*PtrToMemberV).root = ((nd->*PtrToMemberV).root & ~3) | (val + 1); }

    static inline int dec_balance(T* nd) noexcept
    {
        int bp1 = (nd->*PtrToMemberV).root & 3; // the balance + 1
        if (bp1 == 0) return -2; else { --(nd->*PtrToMemberV).root; return bp1 - 2; }
    }

    static inline int inc_balance(T* nd) noexcept
    {
        int bp1 = (nd->*PtrToMemberV).root & 3; // the balance + 1
        if (bp1 == 2) return 2; else { ++(nd->*PtrToMemberV).root; return bp1; }
    }
};

template <class T, class ... OptionsT>
struct tree_taits
{
    using size_type_trait = find_option_t<size_type_tag, intrusive::size_type<size_t>, OptionsT...>;
    using size_type = typename size_type_trait::type;

    using constant_time_size_trait = find_option_t<constant_time_size_tag, constant_time_size<true>, OptionsT...>;
    using size_plugin_t = size_plugin<typename size_type_trait::type, constant_time_size_trait::value>;

    using hook_descriptor_t = find_option_t<hook_tag, void, OptionsT...>;
    using hook_t = hook<T, hook_descriptor_t>;

    using compare_t = find_option_t<compare_tag, basic_compare<std::less<>>, OptionsT...>;
};

template <class T, class HookT>
class tree_iterator
{
public:
    tree_iterator() : node_{ nullptr } {}
    explicit tree_iterator(T* nd) : node_{ nd } {}

    template <typename OT, std::enable_if_t<std::is_same_v<std::add_const_t<OT>, T>, int> = 0>
    tree_iterator(tree_iterator<OT, HookT> const& rhs) : node_{ rhs.node_} {}

    template <typename OT, std::enable_if_t<std::is_same_v<std::add_const_t<T>, OT>, int> = 0>
    tree_iterator& operator= (tree_iterator<OT, HookT> const& rhs)
    {
        node_ = rhs.node_;
        return *this;
    }

    T& operator*() const { return *node_; }
    T* operator->() const { return node_; }

    bool empty() const { return !node_; }

    bool operator== (tree_iterator const& rhs) const { return node_ == rhs.node_; }
    bool operator!= (tree_iterator const& rhs) const { return node_ != rhs.node_; }

    tree_iterator& operator++()
    {
        T* r = HookT::right(node_);
        if (r) {
            do {
                node_ = r;
                r = HookT::left(node_);
            } while(r);
        } else {
            for (;;) {
                r = HookT::get_root(node_);
                if (r) {
                    if (HookT::right(r) == node_) {
                        node_ = r;
                        continue;
                    } else {
                        assert (HookT::left(r) == node_);
                        node_ = r;
                        break;
                    }
                }
                node_ = nullptr;
                break;
            }
        }

        return *this;
    }

private:
    T* node_;
};

} // avl_detail

//typedef avl_multiset< MyClass, MemberOption>   MemberMultiset;


template <class T, class ... OptionsT>
class avl_tree
    : public avl_detail::tree_taits<T, OptionsT...>::size_plugin_t
{
    using traits_t = avl_detail::tree_taits<T, OptionsT...>;
    
    using hook_t = typename traits_t::hook_t;

public:
    using node = T;
    using bstree_t = bstree<T, hook_t>;
    using size_type = size_t;
    using iterator = avl_detail::tree_iterator<T, hook_t>;
    using const_iterator = avl_detail::tree_iterator<T, hook_t>;
    using compare_t = typename traits_t::compare_t;

    T* root() const noexcept { return root_; }
    T* first() const noexcept { return bstree_t::first(root_); }

    iterator begin() noexcept { return iterator{ bstree_t::first(root_) }; }
    const_iterator begin() const noexcept  { return const_iterator{ bstree_t::first(root_) }; }
    const_iterator cbegin() const noexcept { return const_iterator{ bstree_t::first(root_) }; }

    iterator end() noexcept { return iterator{}; }
    const_iterator end() const noexcept  { return const_iterator{}; }
    const_iterator cend() const noexcept { return const_iterator{}; }

    bool empty() const noexcept { return !root_; }

    T* leftmost() noexcept { return bstree_t::leftmost(root_); }
    unsigned check_structure() const { return check(root_); }
    T * unlink_without_rebalance(T * nd); // returns parent node

    static T* next(T*) noexcept;

    template <typename KT>
    const_iterator find(KT const& k) const { return const_iterator { bstree_t::find(root_, k, compare_t()) }; }

    template <typename KT>
    iterator find(KT const& k) { return iterator{ bstree_t::find(root_, k, compare_t()) }; }

    template <typename KT>
    T const * lower_bound(KT const& k) const { return bstree_t::lower_bound(root_, k, compare_t()); }

    template <typename KT>
    T * lower_bound(KT const& k) { return bstree_t::lower_bound(root_, k, compare_t()); }

    std::pair<T*, bool> insert(T & elem);
    T* insert_before(T * lb, T& elem);

    template <typename KT>
    size_type erase(KT const& k);

    T* remove_node(T&) noexcept;

private:
    void do_balance_for_insert(T* nd, int nd_balance) noexcept;
    void do_balance_for_erase(T* nd, int nd_balance) noexcept;

    void rotate_right(T* root, T* left, int r_balance) noexcept;
    T* big_rotate_right(T* root, T* mleft, T* bright) noexcept;
    void rotate_left(T* root, T* right, int r_balance) noexcept;
    T* big_rotate_left(T* root, T* mright, T* bleft) noexcept;

    void do_insert(T* nd, int cmp, T* pelem); // cmp = compare_t()(*pelem, *nd);

    static unsigned check(T* elem);

private:
    T * root_{nullptr};
};



template <class T, class ... OptionsT>
unsigned avl_tree<T, OptionsT...>::check(T * elem)
{
    if (!elem) return 0;
    unsigned left = check(hook_t::left(elem));
    unsigned right = check(hook_t::right(elem));
    int b = (int)left - (int)right;
    assert(hook_t::get_balance(elem) == b);
    return (left > right ? left : right) + 1;
}

template <class T, class ... OptionsT>
T* avl_tree<T, OptionsT...>::unlink_without_rebalance(T* nd)
{
    T * r = hook_t::get_root(nd);
    if (r) {
        if (hook_t::left(r) == nd) {
            hook_t::left(r) = nullptr;
        } else {
            hook_t::right(r) = nullptr;
        }
    }
    return r;
}

template <class T, class ... OptionsT>
T* avl_tree<T, OptionsT...>::insert_before(T* lb, T & elem)
{
    if (!lb) {
        return insert(elem).first;
    }
    int cmp = compare_t()(elem, *lb);
    if (cmp > 0) return insert(elem).first;

    if (!cmp) return lb;
    T* nextnd = hook_t::left(lb);
    while (nextnd) {
        lb = nextnd;
        cmp = compare_t()(elem, *lb);
        if (cmp < 0)
            nextnd = hook_t::left(lb);
        else if (cmp > 0)
            nextnd = hook_t::right(lb);
        else
            return lb;
    }

    T * pelem = std::addressof(elem);
    do_insert(lb, cmp, pelem);
    return pelem;
}

template <class T, class ... OptionsT>
void avl_tree<T, OptionsT...>::do_insert(T * nd, int cmp, T * elem)
{
    hook_t::left(elem) = hook_t::right(elem) = nullptr;
    hook_t::set_balance(elem, 0);

    hook_t::set_root(elem, nd);

    int nd_balance;
    if (cmp < 0) {
        hook_t::left(nd) = elem;
        nd_balance = hook_t::inc_balance(nd);
    } else {
        hook_t::right(nd) = elem;
        nd_balance = hook_t::dec_balance(nd);
    }

    if (nd_balance) {
        assert(-1 == nd_balance || 1 == nd_balance);
        do_balance_for_insert(nd, nd_balance);
    }

    this->increment_size();
}

template <class T, class ... OptionsT>
std::pair<T*, bool> avl_tree<T, OptionsT...>::insert(T & elem)
{
    if (!root_) {
        root_ = std::addressof(elem);
        hook_t::left(root_) = hook_t::right(root_) = nullptr;
        hook_t::set_balance(root_, 0);
        hook_t::set_root(root_, nullptr);
        this->increment_size();
        return { root_, true };
    }

    T * nd, * nextnd = root_;
    
    int cmp;
    do {
        nd = nextnd;
        cmp = compare_t()(elem, *nd);
        if (cmp < 0)
            nextnd = hook_t::left(nd);
        else if (cmp > 0)
            nextnd = hook_t::right(nd);
        else
            return {nd, false};
    } while (nextnd);

    T * pelem = std::addressof(elem);
    do_insert(nd, cmp, pelem);
    return {pelem, true};
}

template <class T, class ... OptionsT>
void avl_tree<T, OptionsT...>::do_balance_for_insert(T * nd, int nd_balance) noexcept
{
    for (;;) {
        T * nextnd = nd;
        nd = hook_t::get_root(nextnd);
        if (!nd) {
            root_ = nextnd;
            break;
        }
        if (!nd_balance) break;

        int next_balance = (hook_t::left(nd) == nextnd) ? hook_t::inc_balance(nd) : hook_t::dec_balance(nd);
        
        if (2 == next_balance) {
            if (nd_balance < 0) {
                nd = big_rotate_right(nd, nextnd, hook_t::right(nextnd));
            } else {
                assert (nd_balance == 1);
                rotate_right(nd, nextnd, nd_balance);
                nd = nextnd;
            }
            nd_balance = 0;
        } else if (-2 == next_balance) {
            if (nd_balance > 0) {
                nd = big_rotate_left(nd, nextnd, hook_t::left(nextnd));
            } else {
                assert (nd_balance == -1);
                rotate_left(nd, nextnd, nd_balance);
                nd = nextnd;
            }
            nd_balance = 0;
        } else {
            nd_balance = next_balance;
            continue;
        }
    }
}

template <class T, class ... OptionsT>
template <typename KT>
typename avl_tree<T, OptionsT...>::size_type avl_tree<T, OptionsT...>::erase(KT const& k)
{
    auto it = find(k);
    if (!it.empty()) {
        remove_node(*it);
        return 1;
    }
    return 0;
}

template <class T, class ... OptionsT>
T* avl_tree<T, OptionsT...>::remove_node(T & e) noexcept
{
    this->decrement_size();

    T * result_node;
    T * node_to_balance;
    int nd_balance;

    T * rnode = std::addressof(e);
    T * l = hook_t::left(rnode);
    T * r = hook_t::right(rnode);
    
    if (r) {
        result_node = r;
        T* next_after_e = hook_t::left(result_node);
        if (next_after_e) {

            do {
                result_node = next_after_e;
                next_after_e = hook_t::left(result_node);
            } while (next_after_e);

            node_to_balance = hook_t::get_root(result_node);
            nd_balance = hook_t::dec_balance(node_to_balance);

            // disconnect result_node, put its right child on its place
            if (T* next_next_after_e = hook_t::right(result_node); next_next_after_e) {
                assert (0 == hook_t::get_balance(next_next_after_e));
                assert (-1 == hook_t::get_balance(result_node));
                hook_t::set_root(next_next_after_e, node_to_balance);
                hook_t::left(node_to_balance) = next_next_after_e;
            } else {
                hook_t::left(node_to_balance) = nullptr;
            }

            hook_t::right(result_node) = r;
            hook_t::set_root(r, result_node);
            hook_t::set_balance(result_node, hook_t::get_balance(rnode));

        } else {
            hook_t::set_balance(result_node, hook_t::get_balance(rnode));
            node_to_balance = result_node;
            nd_balance = hook_t::inc_balance(node_to_balance);
        }

        // move result_node on the e place
        T* sup_root = hook_t::get_root(rnode);
        hook_t::set_root(result_node, sup_root);
        if (sup_root) {
            if (hook_t::left(sup_root) == rnode) {
                hook_t::left(sup_root) = result_node;
            } else {
                assert(hook_t::right(sup_root) == rnode);
                hook_t::right(sup_root) = result_node;
            }
        } else {
            root_ = result_node;
        }

        hook_t::left(result_node) = l;
        if (l) hook_t::set_root(l, result_node);
        
    } else {
        result_node = node_to_balance = hook_t::get_root(rnode);
        if (l) {

            //    e
            //   / \
            //  l   
            // / \

            // just replace e by l
            assert(1 == hook_t::get_balance(rnode));
            assert(0 == hook_t::get_balance(l));
            hook_t::set_root(l, result_node);
        }

        if (node_to_balance) {
            if (hook_t::left(node_to_balance) == rnode) {
                hook_t::left(node_to_balance) = l;
                nd_balance = hook_t::dec_balance(node_to_balance);
            } else {
                assert(hook_t::right(node_to_balance) == rnode);
                hook_t::right(node_to_balance) = l;
                nd_balance = hook_t::inc_balance(node_to_balance);

                // fix result node
                for (T* prev = result_node;; prev = result_node) {
                    result_node = hook_t::get_root(prev);
                    if (!result_node || hook_t::left(result_node) == prev) break;
                }
            }
        } else {
            root_ = l;
            return nullptr;
        }
    }
    do_balance_for_erase(node_to_balance, nd_balance);
    return result_node;
}

template <class T, class ... OptionsT>
void avl_tree<T, OptionsT...>::do_balance_for_erase(T* nd, int nd_balance) noexcept
{
    for (;;)
    {
        if (!nd_balance) {
            T* r = hook_t::get_root(nd);
            if (r) {
                if (hook_t::left(r) == nd) {
                    nd_balance = hook_t::dec_balance(r);
                } else {
                    assert (hook_t::right(r) == nd);
                    nd_balance = hook_t::inc_balance(r);
                }
                nd = r;
                continue;
            }
            root_ = nd;
            return;
        } else if (-2 == nd_balance) {
            T * nextnd = hook_t::right(nd);
            int nextnd_balance = hook_t::get_balance(nextnd);
            if (nextnd_balance > 0) {
                nd = big_rotate_left(nd, nextnd, hook_t::left(nextnd));
                nd_balance = 0;
            } else {
                assert(nextnd_balance <= 0);
                rotate_left(nd, nextnd, nextnd_balance);
                nd = nextnd;
                nd_balance = 1 + nextnd_balance; // -1->0, 0->1
            }
        } else if (2 == nd_balance) {
            T* nextnd = hook_t::left(nd);
            int nextnd_balance = hook_t::get_balance(nextnd);
            if (nextnd_balance < 0) {
                nd = big_rotate_right(nd, nextnd, hook_t::right(nextnd));
                nd_balance = 0;
            } else {
                assert(nextnd_balance >= 0);
                rotate_right(nd, nextnd, nextnd_balance);
                nd = nextnd;
                nd_balance = nextnd_balance - 1; // 1->0, 0->-1
            }
        } else {
            assert (1 == nd_balance || -1 == nd_balance);
            if (!hook_t::get_root(nd)) {
                root_ = nd;
            }
            return;
        }
    }
}

//           root               left   
//          /    \             /     \
//        left    r    -->    p     root 
//       /    \                    /    \
//     p       q                  q      r

template <class T, class ... OptionsT>
void avl_tree<T, OptionsT...>::rotate_right(T * root, T * left, int l_balance) noexcept
{
    assert (1 == hook_t::get_balance(root));

    T* q = hook_t::right(left);
    hook_t::left(root) = q;
    if (q) hook_t::set_root(q, root);

    hook_t::right(left) = root;
    T* sup_root = hook_t::get_root(root);
    hook_t::set_root(left, sup_root);
    hook_t::set_root(root, left);
    hook_t::set_balance(root, 1 - l_balance); // 1->0, 0->1
    hook_t::set_balance(left, l_balance - 1); // 1->0, 0->-1
    if (sup_root) {
        if (hook_t::left(sup_root) == root) {
            hook_t::left(sup_root) = left;
        } else {
            assert (hook_t::right(sup_root) == root);
            hook_t::right(sup_root) = left;
        }
    }
}

//           root                   bright   
//          /    \                 /     \
//       mleft    s             mleft    root 
//       /   \          -->    /   \     /  \
//     p     bright           p     q   r    s
//            /  \
//           q    r

template <class T, class ... OptionsT>
T* avl_tree<T, OptionsT...>::big_rotate_right(T* root, T* mleft, T* bright) noexcept
{
    assert (1 == hook_t::get_balance(root));
    assert (-1 == hook_t::get_balance(mleft));
    T * q = hook_t::left(bright); // can be null if r is not null
    T * r = hook_t::right(bright); // can be null if q is not null
    hook_t::right(mleft) = q;
    if (q) hook_t::set_root(q, mleft);
    hook_t::left(root) = r;
    if (r) hook_t::set_root(r, root);

    T* sup_root = hook_t::get_root(root);
    hook_t::set_root(bright, sup_root);
    hook_t::left(bright) = mleft; hook_t::set_root(mleft, bright);
    hook_t::right(bright) = root; hook_t::set_root(root, bright);
    if (sup_root) {
        if (hook_t::left(sup_root) == root) {
            hook_t::left(sup_root) = bright;
        } else {
            assert (hook_t::right(sup_root) == root);
            hook_t::right(sup_root) = bright;
        }
    }

    if (1 == hook_t::get_balance(bright)) {
        hook_t::set_balance(mleft, 0);
        hook_t::set_balance(root, -1);
    } else if (-1 == hook_t::get_balance(bright)) {
        hook_t::set_balance(mleft, 1);
        hook_t::set_balance(root, 0);
    } else {
        hook_t::set_balance(mleft, 0);
        hook_t::set_balance(root, 0);
    }
    hook_t::set_balance(bright, 0);
    return bright;
}

//       root                   right   
//      /    \                 /     \
//     p    right     -->    root     r
//         /     \          /    \
//        q       r        p      q

template <class T, class ... OptionsT>
void avl_tree<T, OptionsT...>::rotate_left(T* root, T* right, int r_balance) noexcept
{
    assert (-1 == hook_t::get_balance(root));
    assert (-1 == r_balance || 0 == r_balance);

    T* q = hook_t::left(right);
    hook_t::right(root) = q;
    if (q) hook_t::set_root(q, root);

    hook_t::left(right) = root;
    T* sup_root = hook_t::get_root(root);
    hook_t::set_root(right, sup_root);
    hook_t::set_root(root, right);
    hook_t::set_balance(root,  -1 - r_balance);   // -1->0, 0->-1
    hook_t::set_balance(right, 1 + r_balance); // -1->0, 0->1
    if (sup_root) {
        if (hook_t::right(sup_root) == root) {
            hook_t::right(sup_root) = right;
        } else {
            assert (hook_t::left(sup_root) == root);
            hook_t::left(sup_root) = right;
        }
    }
}

//           root                    bleft   
//          /    \                  /     \
//         p   mright             root    mright 
//             /    \     -->    /   \    /   \
//          bleft    s          p     q  r     s
//         /     \
//        q       r

template <class T, class ... OptionsT>
T* avl_tree<T, OptionsT...>::big_rotate_left(T* root, T* mright, T* bleft) noexcept
{
    assert (-1 == hook_t::get_balance(root));
    assert (1 == hook_t::get_balance(mright));
    T* q = hook_t::left(bleft); // can be null if r is not null
    T* r = hook_t::right(bleft); // can be null if q is not null
    hook_t::left(mright) = r;
    if (r) hook_t::set_root(r, mright);
    hook_t::right(root) = q;
    if (q) hook_t::set_root(q, root);

    T* sup_root = hook_t::get_root(root);
    hook_t::set_root(bleft, sup_root);
    hook_t::right(bleft) = mright; hook_t::set_root(mright, bleft);
    hook_t::left(bleft) = root; hook_t::set_root(root, bleft);

    if (sup_root) {
        if (hook_t::left(sup_root) == root) {
            hook_t::left(sup_root) = bleft;
        } else {
            assert (hook_t::right(sup_root) == root);
            hook_t::right(sup_root) = bleft;
        }
    }

    if (-1 == hook_t::get_balance(bleft)) {
        hook_t::set_balance(mright, 0);
        hook_t::set_balance(root, 1);
    } else if (1 == hook_t::get_balance(bleft)) {
        hook_t::set_balance(mright, -1);
        hook_t::set_balance(root, 0);
    } else {
        hook_t::set_balance(mright, 0);
        hook_t::set_balance(root, 0);
    }
    hook_t::set_balance(bleft, 0);
    return bleft;
}

}

#endif // AGNOSTIC_INTRUSIVE_AVL_HPP
