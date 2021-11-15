/*=============================================================================
    Copyright (c) 2005-2006 Alexander A Potocki

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#pragma once

#include "composite.hpp"

#include <boost/intrusive_ptr.hpp>

#include "sonia/type_traits.hpp"
#include "sonia/optional.hpp"

#include "auxilary/dyn_input_iterator.hpp"
#include "auxilary/dyn_output_iterator.hpp"
#include "auxilary/dyn_state.hpp"

namespace sonia::conversion {

namespace detail {

template <typename objT>
void intrusive_ptr_add_ref(objT * pobj) { pobj->internal_add_ref(); }
template <typename objT>
void intrusive_ptr_release(objT * pobj) { pobj->internal_release(); }

template <typename src_t, typename dest_t>
class coder_impl
{
public:
    typedef auxilary::dyn_state dyn_state;
    typedef auxilary::dyn_ref_fwd_input_iterator<src_t> iterator_t;
    typedef auxilary::dyn_ref_fwd_output_iterator<dest_t> oiterator_t;

    typedef std::pair<iterator_t, iterator_t> range_t;

    virtual void create_gstate(dyn_state &) const = 0;
    virtual void create_pstate(dyn_state &) const = 0;

    virtual void clear_gstate(dyn_state &) const = 0;
    virtual void clear_pstate(dyn_state &) const = 0;

    virtual bool equal_gstates(dyn_state const& s1, dyn_state const& s2) const = 0;
    virtual bool equal_pstates(dyn_state const& s1, dyn_state const& s2) const = 0;

    coder_impl() : referencies_(0) {}

    template <class ForwardWritableIterator>
    void put(src_t c, ForwardWritableIterator& out, dyn_state& s) const
    {
        auxilary::concrete_nc_output_iterator_reference<dest_t, ForwardWritableIterator> from(out);
        auxilary::dyn_ref_fwd_output_iterator<dest_t> ref_it(from);
        return do_put(c, ref_it, s);
    }

    template <class ForwardWritableIterator>
    void unshift(ForwardWritableIterator& out, dyn_state& s) const
    {
        auxilary::concrete_nc_output_iterator_reference<dest_t, ForwardWritableIterator> from(out);
        auxilary::dyn_ref_fwd_output_iterator<dest_t> ref_it(from);
        return do_unshift(ref_it, s);
    }

    template <typename iteratorT>
    inline optional<dest_t> get(std::pair<iteratorT, iteratorT>& in, dyn_state& s) const
    {
        auxilary::concrete_input_iterator_reference<src_t, iteratorT> first(in.first);
        auxilary::concrete_input_iterator_reference<src_t, iteratorT> second(in.second);
        range_t rng = std::make_pair(iterator_t(first), iterator_t(second));
        return do_get(rng, s);
    }

    template <typename iteratorT, typename ForwardWritableIterator>
    inline ForwardWritableIterator get_range(std::pair<iteratorT, iteratorT>& in, dyn_state & s, ForwardWritableIterator beginDest, ForwardWritableIterator endDest) const
    {
        auxilary::concrete_input_iterator_reference<src_t, iteratorT> first(in.first);
        auxilary::concrete_input_iterator_reference<src_t, iteratorT> second(in.second);
        range_t rng = std::make_pair(iterator_t(first), iterator_t(second));
        auxilary::concrete_output_iterator_reference<dest_t, ForwardWritableIterator> ofirst(beginDest);
        auxilary::concrete_output_iterator_reference<dest_t, ForwardWritableIterator> osecond(endDest);
        oiterator_t oitf(ofirst);
        oiterator_t oits(osecond);
        do_get_range(rng, s, oitf, oits);
        return beginDest;
    }

    virtual ~coder_impl(){}

protected:
    virtual void do_put(src_t c, oiterator_t & out, dyn_state& s) const = 0;
    virtual void do_unshift(auxilary::dyn_ref_fwd_output_iterator<dest_t> & out, dyn_state& s) const = 0;
    virtual optional<dest_t> do_get(range_t & in, dyn_state& s) const = 0;
    virtual oiterator_t do_get_range(range_t & in, dyn_state & s, oiterator_t & from, oiterator_t & to) const = 0;

private:
    friend void intrusive_ptr_add_ref<coder_impl>(coder_impl*);
    friend void intrusive_ptr_release<coder_impl>(coder_impl*);

    inline void internal_add_ref() const
    { referencies_++;}

    inline void internal_release() const
    {
        if (--referencies_ <= 0) {
            delete this;
        }
    } 

    mutable long referencies_;
};

template <typename EncoderT>
class concrete_coder_impl : public coder_impl<typename EncoderT::source_type, typename EncoderT::destination_type>
{
    typedef coder_impl<typename EncoderT::source_type, typename EncoderT::destination_type> base_type;
    typedef auxilary::dyn_state dyn_state;
    typedef typename base_type::iterator_t iterator_t;
    typedef typename base_type::oiterator_t oiterator_t;
    typedef typename EncoderT::destination_type dest_t;
    typedef typename EncoderT::source_type src_t;
    typedef typename EncoderT::gstate_t gstateF;
    typedef typename EncoderT::pstate_t pstateF;
    typedef typename gstateF::type gstate_t;
    typedef typename pstateF::type pstate_t;

public:
    template <typename initTag>
    concrete_coder_impl(initTag const& it) : encoder_(it) {}

    concrete_coder_impl(concrete_coder_impl const& other) : encoder_(other.encoder_) {}

    void create_gstate(dyn_state &s) const
    { s.create<gstate_t>(); }
    
    void create_pstate(dyn_state &s) const
    { s.create<pstate_t>(); }

    void clear_gstate(dyn_state& s) const
    {
        if (s.empty()) {
            s.create<pstate_t>();
        }
        encoder_.gclear(s.get<gstate_t>());
    }

    void clear_pstate(dyn_state& s) const
    {
        if (s.empty()) {
            s.create<pstate_t>();
        }
        encoder_.pclear(s.get<pstate_t>());
    }

    bool equal_gstates(dyn_state const& s1, dyn_state const& s2) const
    {
        return encoder_.equal_gstates(
              s1.get<gstate_t>()
            , s2.get<gstate_t>()
        );
    }

    bool equal_pstates(dyn_state const& s1, dyn_state const& s2) const
    {
        return encoder_.equal_pstates(
              s1.get<pstate_t>()
            , s2.get<pstate_t>()
        );
    }

protected:
    inline void do_put(src_t c, auxilary::dyn_ref_fwd_output_iterator<dest_t> & out, dyn_state& s) const
    {
        return encoder_.put(c, out, s.get<pstate_t>());
    }

    inline void do_unshift(auxilary::dyn_ref_fwd_output_iterator<dest_t> & out, dyn_state& s) const
    {
        return encoder_.unshift(out, s.get<pstate_t>());
    }

    inline optional<dest_t> do_get(std::pair<iterator_t, iterator_t> & in, dyn_state& s) const
    {
        return encoder_.get(in, s.get<gstate_t>());
    }

    inline oiterator_t do_get_range(std::pair<iterator_t, iterator_t> & in, dyn_state & s, oiterator_t & from, oiterator_t & to) const
    {
        return encoder_.get_range(in, s.get<gstate_t>(), from, to);
    }

private:
    EncoderT encoder_;
};

} // namespace sonia::conversion::detail

template <
      typename ProviderT
    , typename CustomerT
>
class encoder
{
public:
    typedef CustomerT destination_type;
    typedef ProviderT source_type;

    typedef auxilary::dyn_state gstate_t;
    typedef auxilary::dyn_state pstate_t;

    encoder()
    {

    }

    template <typename EncExpression>
    encoder(EncExpression const& expr)
    {
        typedef typename EncExpression::template apply<null_t>::type encoder_t;
        encoder_ = new detail::concrete_coder_impl<encoder_t>(expr.get_init_tag());
    }

    encoder( encoder const& other ) : encoder_(other.encoder_) {}
    
    void gclear(gstate_t & s) const
    {
        if (encoder_.get()) {
            if (s.empty()) {
                encoder_->create_gstate(s);
            }
            encoder_->clear_gstate(s);
        }
    }

    void pclear(pstate_t & s) const
    {
        if (encoder_.get()) {
            if (s.empty()) {
                encoder_->create_pstate(s);
            }
            encoder_->clear_pstate(s);
        }
    }

    bool equal_gstates(gstate_t const& s1, gstate_t const& s2) const
    { return encoder_->equal_gstates(s1, s2); }

    bool equal_pstates(pstate_t const& s1, pstate_t const& s2) const
    { return encoder_->equal_pstates(s1, s2); }

    template <class ForwardWritableIterator>
    void put(source_type c, ForwardWritableIterator& out, pstate_t & s) const
    {
        return encoder_->put(c, out, s);
    }

    template <class ForwardWritableIterator>
    void unshift(ForwardWritableIterator& out, pstate_t & s) const
    {
        return encoder_->unshift(out, s);
    }

    template <typename ForwardReadableIterator>
    optional<destination_type> get(ForwardReadableIterator & in, gstate_t & s) const
    {
        if (encoder_.get()) { return encoder_->get(in, s); }
        return optional<destination_type>();
    }

    template <typename ForwardReadableIterator, typename ForwardWritableIterator>
    inline ForwardWritableIterator get_range(ForwardReadableIterator& in, gstate_t & s, ForwardWritableIterator beginDest, ForwardWritableIterator endDest) const
    {
        if (encoder_.get()) { return encoder_->get_range(in, s, beginDest, endDest); }
        return beginDest;
    }

private:
    boost::intrusive_ptr<detail::coder_impl<source_type, destination_type> > encoder_;
};

// forwarding for encoder type
namespace detail {

template <typename pT, typename cT>
class converter_impl<encoder<pT, cT> >
{
public:
    typedef encoder<pT, cT> type;
};
 
}}
