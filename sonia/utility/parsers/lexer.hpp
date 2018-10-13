//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_LEXER_UTILITY_HPP
#define SONIA_LEXER_UTILITY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include "lexertl14/generator.hpp"
#include "lexertl14/lookup.hpp"

#include <boost/utility/addressof.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "sonia/optional.hpp"
#include "sonia/iterator_traits.hpp"

namespace sonia {

template <class LexerT, typename IteratorT>
class lexertl_iterator : public boost::iterator_facade<
      lexertl_iterator<LexerT, IteratorT>
    , lexertl::match_results<IteratorT>
    , boost::forward_traversal_tag
    , lexertl::match_results<IteratorT> const&>
{
    friend class boost::iterator_core_access;

public:
    lexertl_iterator() : plex_(nullptr) {}

    lexertl_iterator(IteratorT b, IteratorT const& e, LexerT const& lex)
        : plex_(boost::addressof(lex)), token_(in_place, b, e)
    {
        plex_->lookup(*token_);
    }

    inline bool empty() const { 
        return !plex_ || token_->first == token_->eoi;
    }

    void operator= (lexertl_iterator const& rhs) {
        plex_ = rhs.plex_;
        token_ = rhs.token_;
    }

    void clear_queue() {}

private:
    bool equal(lexertl_iterator const& rhs) const noexcept {
        if (rhs.empty()) return empty();
        if (empty()) return false;
        return token_->first == rhs.token_->first;
    }

    void increment() {
        plex_->lookup(*token_);
    }

    typename lexertl_iterator::value_type const& dereference() const {
        return *token_;
    }

    LexerT const* plex_;
    optional<typename lexertl_iterator::value_type> token_;
};

template <typename DerivedT, typename IteratorT>
class lexer_base
{
public:
    typedef iterator_value_t<IteratorT> char_t;

    //typedef IteratorT iterator;
    typedef lexertl_iterator<DerivedT, IteratorT> iterator;
    typedef lexertl::match_results<IteratorT> token_type;

    lexer_base() {
        DerivedT::build_rules(rules_);
        lexertl::generator::build(rules_, state_machine_);
    }

    // prototype
    template <typename RulesT>
    static void build_rules(RulesT & rules) {}

    iterator begin(IteratorT b, IteratorT const& e) const {
        return iterator(b, e, static_cast<DerivedT const&>(*this));
    }

    iterator end() const {
        return iterator();
    }

    void lookup(lexertl::match_results<IteratorT> & result) const {
        lexertl::lookup(state_machine_, result);
    }

private:
    lexertl::basic_rules<char_t, char_t> rules_;
    lexertl::basic_state_machine<char_t> state_machine_;
};

}

#endif // SONIA_LEXER_UTILITY_HPP
