//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "linked_list.hpp"

namespace sonia::lang::bang {

template <class ElementT, class ManagerT>
template <typename T>
void managed_linked_list<ElementT, ManagerT>::emplace_back(T&& e)
{
    typename list_t::entry_type& ne = m_->acquire(std::forward<T>(e));
    list_t::push_back(ne);
}

template <class ElementT, class ManagerT>
void managed_linked_list<ElementT, ManagerT>::clear(list_t& l) noexcept
{
    while (l) {
        //typename list_t::entry_type&& e = l.pop_back();
        //GLOBAL_LOG_INFO() << "removing:\n" << u_.print(e.value);
        //if (list_t* pl = get<list_t>(&e.value); pl) {
        //    l.splice_back(*pl, pl->begin(), pl->end());
        //}
        m_->release(l.pop_back());
    }
}

template <class ElementT, class ManagerT>
managed_linked_list<ElementT, ManagerT>::~managed_linked_list()
{
    clear(*this);
}

template <class EntryT>
inline auto& linked_list_node_span<EntryT>::front() const noexcept { return pair_t::first->value; }

template <class EntryT>
inline auto& linked_list_node_span<EntryT>::back() const noexcept { return pair_t::second->value; }

template <class EntryT>
template <typename FtorT> void linked_list_node_span<EntryT>::for_each(FtorT&& ftor) const
{
    for (EntryT* e = pair_t::first;;)
    {
        ftor(e->value);
        if (e == pair_t::second) break;
        e = static_cast<EntryT*>(linked_list<typename EntryT::value_type>::next(*e));
    }
}

//template <class EntryT>
//inline void linked_list_node_span<EntryT>::emplace_back(EntryT* se)
//{
//    if (!pair_t::first) [[unlikely]] {
//        pair_t::first = se;
//    } else {
//        BOOST_ASSERT(pair_t::second->next_ == se);
//        pair_t::second = se;
//    }
//}

template <class EntryT>
inline void linked_list_node_span<EntryT>::pop_front()
{
    BOOST_ASSERT(pair_t::first);
    if (pair_t::first == pair_t::second) {
        pair_t::first = pair_t::second = nullptr;
    } else {
        pair_t::first = static_cast<EntryT*>(linked_list<typename EntryT::value_type>::next(*pair_t::first));
    }
}

template <class EntryT>
inline linked_list_node_span<EntryT> linked_list_node_span<EntryT>::subspan(size_t n) const
{
    linked_list_node_span<EntryT> res = *this;
    while (n-- > 0) {
        res.pop_front();
    }
    return res;
}

}
