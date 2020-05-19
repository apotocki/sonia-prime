// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License

#ifndef AGNOSTIC_INTRUSIVE_BSTREE_HPP
#define AGNOSTIC_INTRUSIVE_BSTREE_HPP

#pragma once

namespace agnostic::intrusive {

template <typename NodeT, typename HookT>
class bstree
{
public:
    using node = NodeT;

    static NodeT* first(NodeT* nd) noexcept;
    static NodeT* leftmost(NodeT* r) noexcept; // returns the lestmost node from the given root node
    static NodeT* last(NodeT* nd) noexcept;
    static NodeT* root(NodeT* nd) noexcept;

    static NodeT* next(NodeT* nd) noexcept;
    static NodeT* previous(NodeT* nd) noexcept;

    template <typename KeyT, typename CompareT>
    static NodeT* find(NodeT * from, KeyT const& key, CompareT const& cftor);

    template <typename KeyT, typename CompareT>
    static NodeT * lower_bound(NodeT* from, KeyT const& key, CompareT const& cftor);
};

template <typename NodeT, typename HookT>
NodeT* bstree<NodeT, HookT>::first(NodeT* nd) noexcept
{
    if (nd) {
        for (;;) {
            NodeT* nextnd = HookT::left(nd);
            if (nextnd) {
                nd = nextnd;
                continue;
            }
            break;
        }
    }
    return nd;
}

template <typename NodeT, typename HookT>
NodeT* bstree<NodeT, HookT>::leftmost(NodeT* r) noexcept
{
    NodeT* result = nullptr;
    while (r) {
        result = first(r);
        r = HookT::right(result);
    };
    return result;
}

template <typename NodeT, typename HookT>
NodeT* bstree<NodeT, HookT>::last(NodeT* nd) noexcept
{
    if (nd) {
        for (;;) {
            NodeT* nextnd = HookT::right(nd);
            if (nextnd) {
                nd = nextnd;
                continue;
            }
            break;
        }
    }
    return nd;
}

template <typename NodeT, typename HookT>
NodeT* bstree<NodeT, HookT>::root(NodeT* nd) noexcept
{
    if (nd) {
        for (;;) {
            NodeT* nextnd = HookT::get_root(nd);
            if (nextnd) {
                nd = nextnd;
                continue;
            }
            break;
        }
    }
    return nd;
}

template <typename NodeT, typename HookT>
NodeT* bstree<NodeT, HookT>::next(NodeT * nd) noexcept
{
    NodeT* r = HookT::right(nd);
    if (r) {
        do {
            nd = r;
            r = HookT::left(nd);
        } while (r);
    } else {
        for (;;) {
            r = HookT::get_root(nd);
            if (r) {
                if (HookT::right(r) == nd) {
                    nd = r;
                    continue;
                }
                else {
                    assert (HookT::left(r) == nd);
                    nd = r;
                    break;
                }
            }
            nd = nullptr;
            break;
        }
    }
    return nd;
}

template <typename NodeT, typename HookT>
NodeT* bstree<NodeT, HookT>::previous(NodeT* nd) noexcept
{
    NodeT* l = HookT::left(nd);
    if (l) {
        do {
            nd = l;
            l = HookT::right(nd);
        } while (l);
    } else {
        for (;;) {
            l = HookT::get_root(nd);
            if (l) {
                if (HookT::left(l) == nd) {
                    nd = l;
                    continue;
                } else {
                    assert(HookT::right(l) == nd);
                    nd = l;
                    break;
                }
            }
            nd = nullptr;
            break;
        }
    }
    return nd;
}

template <typename NodeT, typename HookT>
template <typename KeyT, typename CompareT>
static NodeT* bstree<NodeT, HookT>::find(NodeT* from, KeyT const& key, CompareT const& cftor)
{
    while (from) {
        int cmp = cftor(key, *from);
        if (cmp < 0)
            from = HookT::left(from);
        else if (cmp > 0)
            from = HookT::right(from);
        else
            break;
    }
    return from;
}

template <typename NodeT, typename HookT>
template <typename KeyT, typename CompareT>
static NodeT* bstree<NodeT, HookT>::lower_bound(NodeT* from, KeyT const& key, CompareT const& cftor)
{
    NodeT* lb = nullptr;
    while (from) {
        int cmp = cftor(key, *from);
        if (cmp < 0) {
            lb = from;
            from = HookT::left(from);
        }
        else if (cmp > 0) {
            from = HookT::right(from);
        }
        else {
            return from;
        }
    }
    return lb;
}

}

#endif // AGNOSTIC_INTRUSIVE_BSTREE_HPP
