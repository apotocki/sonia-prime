//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_LEXER_UTILITY_HPP
#define SONIA_LEXER_UTILITY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "lexertl/generator.hpp"
#include "lexertl/lookup.hpp"

#include <boost/utility/addressof.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/optional.hpp"
#include "sonia/iterator_traits.hpp"

namespace sonia {

template <class LexerT, typename IteratorT, typename ValueT>
class lexertl_iterator
    : public boost::iterator_facade<
          lexertl_iterator<LexerT, IteratorT, ValueT>
        , ValueT
        , boost::forward_traversal_tag
        , ValueT const&>
{
    friend class boost::iterator_core_access;

public:
    lexertl_iterator() : plex_(nullptr) {}

    lexertl_iterator(IteratorT b, IteratorT const& e, LexerT const& lex)
        : plex_(boost::addressof(lex)), token_(in_place, b, e)
    {
        plex_->lookup(*token_);
    }

    inline bool empty() const
    { 
        return !plex_ || token_->first == token_->eoi;
    }

    void operator= (lexertl_iterator const& rhs)
    {
        plex_ = rhs.plex_;
        token_ = rhs.token_;
    }

    void clear_queue() {}

private:
    bool equal(lexertl_iterator const& rhs) const noexcept
    {
        if (rhs.empty()) return empty();
        if (empty()) return false;
        return token_->first == rhs.token_->first;
    }

    void increment()
    {
        plex_->lookup(*token_);
    }

    typename lexertl_iterator::value_type const& dereference() const { return *token_; }

    LexerT const* plex_;
    optional<ValueT> token_;
};

template <typename DerivedT, typename IteratorT, bool RecursiveV = false>
class lexer_base
{
public:
    using char_t = iterator_value_t<IteratorT>;
    using token_type = conditional_t<RecursiveV, lexertl::recursive_match_results<IteratorT>, lexertl::match_results<IteratorT>>;
    using iterator = lexertl_iterator<DerivedT, IteratorT, token_type>;

    lexer_base()
    {
        DerivedT::build_rules(rules_);
        lexertl::generator::build(rules_, state_machine_);
    }

    // prototype
    template <typename RulesT>
    static void build_rules(RulesT & rules) {}

    iterator begin(IteratorT b, IteratorT const& e) const
    {
        return iterator(b, e, static_cast<DerivedT const&>(*this));
    }

    iterator end() const { return iterator(); }

    void lookup(token_type & result) const
    {
        lexertl::lookup(state_machine_, result);
    }

private:
    lexertl::basic_rules<char_t, char_t> rules_;
    lexertl::basic_state_machine<char_t> state_machine_;
};

}

namespace boost {

template <typename iter, typename id_type, std::size_t flags>
struct range_iterator<lexertl::match_results<iter, id_type, flags>>
{
    using type = iter;
};

template <typename iter, typename id_type, std::size_t flags>
struct range_iterator<const lexertl::match_results<iter, id_type, flags>>
{
    using type = iter;
};

}

namespace lexertl {

template <typename iter, typename id_type, std::size_t flags>
inline iter range_begin(match_results<iter, id_type, flags> const& mr)
{
    return mr.first;
}

template <typename iter, typename id_type, std::size_t flags>
inline iter range_end(match_results<iter, id_type, flags> const& mr)
{
    return mr.second;
}

}

#endif // SONIA_LEXER_UTILITY_HPP
