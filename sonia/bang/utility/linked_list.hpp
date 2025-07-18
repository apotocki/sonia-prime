//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <type_traits>
#include <utility>

#include <boost/intrusive/list.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "sonia/exceptions.hpp"

namespace sonia::lang::bang {

template <class ElementT>
class linked_list_node : public boost::intrusive::list_base_hook<>
{
public:
    using value_type = ElementT;
    //boost::intrusive::list_member_hook<> member_hook_;
    ElementT value;
    template <typename ArgT>
    requires (!std::is_same_v<linked_list_node, std::remove_cvref_t<ArgT>>)
    inline explicit linked_list_node(ArgT&& arg) : value{ std::forward<ArgT>(arg) } {}

    linked_list_node(linked_list_node const&) = delete;
    linked_list_node& operator=(linked_list_node const&) = delete;
};

template <class EntryT>
struct linked_list_node_span : std::pair<EntryT*, EntryT*>
{
    using pair_t = std::pair<EntryT*, EntryT*>;
    inline linked_list_node_span() noexcept : pair_t{ nullptr, nullptr } {}
    inline explicit linked_list_node_span(EntryT* b) noexcept : pair_t{ b, b } {}
    inline linked_list_node_span(EntryT* b, EntryT* e) noexcept : pair_t{ b, e } { BOOST_ASSERT(b && e); }

    template <typename BaseEntryT>
    requires std::is_base_of_v<BaseEntryT, EntryT>
    inline explicit linked_list_node_span(BaseEntryT* b) noexcept
        : pair_t{ static_cast<EntryT*>(b), static_cast<EntryT*>(b) }
    {}

    template <typename BaseEntryT>
    requires std::is_base_of_v<BaseEntryT, EntryT>
    inline linked_list_node_span(BaseEntryT* b, BaseEntryT* e) noexcept
        : pair_t{ static_cast<EntryT*>(b), static_cast<EntryT*>(e) }
    {
        BOOST_ASSERT(b && e);
    }

    auto& front() const noexcept;
    auto& back() const noexcept;

    inline bool empty() const noexcept { return pair_t::first == nullptr; }
    inline explicit operator bool() const noexcept { return pair_t::first != nullptr; }

    template <typename FtorT> void for_each(FtorT&& ftor) const;

    //void emplace_back(EntryT*);
    void pop_front();
    linked_list_node_span subspan(size_t) const;
};

template <class ElementT>
class linked_list
{
public:
    using entry_type = linked_list_node<ElementT>;

private:
    using list_t = boost::intrusive::list<
        entry_type,
        //boost::intrusive::member_hook<entry, boost::intrusive::list_member_hook<>, &entry::member_hook_>>;
        boost::intrusive::base_hook<entry_type>>;

    list_t list_;

    static ElementT const& to_cexpr(entry_type const& e) noexcept { return e.value; }
    static ElementT & to_expr(entry_type& e) noexcept { return e.value; }

public:
    using iterator = boost::transform_iterator<decltype(&to_expr), typename list_t::iterator>;
    using const_iterator = boost::transform_iterator<decltype(&to_cexpr), typename list_t::const_iterator>;

    linked_list() noexcept = default;
    linked_list(linked_list const&) = delete;
    linked_list(linked_list &&) noexcept = default;
    linked_list& operator=(linked_list const&) = delete;
    linked_list& operator=(linked_list&& rhs) noexcept = default;
    
    inline explicit operator bool() const noexcept { return !list_.empty(); }

    inline const_iterator begin() const noexcept { return const_iterator{ list_.begin(), to_cexpr }; }
    inline iterator begin() noexcept { return iterator{ list_.begin(), to_expr }; }
    inline const_iterator end() const noexcept { return const_iterator{ list_.end(), to_cexpr }; }
    inline iterator end() noexcept { return iterator{ list_.end(), to_expr }; }
    inline const_iterator last() const noexcept { const_iterator r = end(); if (!list_.empty()) --r; return r; }
    inline iterator last() noexcept { iterator r = end(); if (!list_.empty()) --r; return r; }

    inline size_t size() const noexcept { return list_.size(); }
    inline bool empty() const noexcept { return list_.empty(); }

    inline ElementT const& front() const noexcept { return list_.front().value; }
    inline ElementT & front() noexcept { return list_.front().value; }
    inline entry_type& front_entry() noexcept { return list_.front(); }
    inline ElementT const& back() const noexcept { return list_.back().value; }
    inline ElementT & back() noexcept { return list_.back().value; }
    inline entry_type& back_entry() noexcept { return list_.back(); }

    template <std::derived_from<entry_type> EntryT>
    operator linked_list_node_span<EntryT>() noexcept
    {
        if (!empty()) {
            return linked_list_node_span<EntryT>{ static_cast<EntryT*>(std::addressof(front_entry())), static_cast<EntryT*>(std::addressof(back_entry())) };
        }
        return {};
    }

    template <std::derived_from<entry_type> EntryT>
#if 1
    inline void insert(EntryT& where, entry_type& e)
    {
        // check for validity of 'where' entry
        for (auto bit = list_.begin(); bit != list_.end(); ++bit) {
            if (&*bit == &where) {
                list_.insert(++bit, e);
                return;
            }
        }
        THROW_INTERNAL_ERROR("wrong where");
#else
    inline void insert(EntryT & where, entry_type & e) noexcept
    {
        list_.insert(++list_t::s_iterator_to(where), e);
#endif
    }

    inline void push_back(entry_type& e) noexcept { list_.push_back(e); }
    inline entry_type&& pop_back() noexcept { entry_type& be = list_.back(); list_.pop_back(); return std::move(be); }

    inline void splice_back(linked_list& other, const_iterator b, const_iterator e) noexcept
    {
        list_.splice(list_.end(), other.list_, b.base(), e.base());
    }

    template <std::derived_from<entry_type> EntryT>
    inline void splice_back(linked_list& other, linked_list_node_span<EntryT> const& csp) noexcept
    {
        if (csp) {
            list_.splice(list_.end(), other.list_,
                list_t::s_iterator_to(*csp.first),
                ++list_t::s_iterator_to(*csp.second)
            );
        }
    }

    inline void splice_back(linked_list& other) noexcept
    {
        list_.splice(list_.end(), other.list_);
    }

    template <std::derived_from<entry_type> EntryT>
    inline linked_list_node_span<EntryT> concat(linked_list_node_span<EntryT> l, linked_list_node_span<EntryT> r)
    {
        if (!l) return r;
        if (!r) return l;
        // l and r are expected not intersecting
        list_.splice(++list_t::s_iterator_to(*l.second), list_, list_t::s_iterator_to(*r.first), ++list_t::s_iterator_to(*r.second));
        return linked_list_node_span<EntryT>{ l.first, r.second };
    }

    inline static entry_type* next(entry_type& e) noexcept { return std::addressof(*++list_t::s_iterator_to(e)); }
    inline static entry_type* prev(entry_type& e) noexcept { return std::addressof(*--list_t::s_iterator_to(e)); }
};


template <class ElementT, class ManagerT>
class managed_linked_list : public linked_list<ElementT>
{
    using list_t = linked_list<ElementT>;

    ManagerT* m_;

public:
    inline managed_linked_list() noexcept : m_{ nullptr } {}

    inline explicit managed_linked_list(ManagerT& m) noexcept : m_{ std::addressof(m) } {}
    managed_linked_list(managed_linked_list const&) = delete;
    managed_linked_list& operator= (managed_linked_list const&) = delete;

    managed_linked_list(managed_linked_list&&) = default;
    managed_linked_list& operator= (managed_linked_list&&) = default;

    inline void clear() noexcept { clear(*this); }
    void clear(list_t&) noexcept;

    template <typename T>
    void emplace_back(T&& e);

    template <typename NodeT>
    void deep_copy(linked_list_node_span<NodeT> l);

    inline ManagerT* manager() const noexcept { return m_; }

    ~managed_linked_list();
};

} // namespace sonia::lang::bang
