//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_SERIALIZATION_IOSTREAM_HPP
#define SONIA_SERIALIZATION_IOSTREAM_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <utility>
#include <istream>
#include <ostream>

#include <boost/utility/base_from_member.hpp>

#include "sonia/iterator_traits.hpp"

namespace sonia { namespace serialization {

template <class IteratorT, typename ElemT, class TraitsT>
class istreambuf : public std::basic_streambuf<ElemT, TraitsT>
{
    typedef std::basic_streambuf<ElemT, TraitsT> base_t;
    typedef typename base_t::traits_type traits_type;
    typedef typename base_t::int_type int_type;

    static const bool is_read = !is_output_iterator_v<IteratorT>;
        
    using base_t::gptr;
    using base_t::egptr;
    using base_t::gbump;
    using base_t::eback;
    using base_t::setg;
    
public:
    explicit istreambuf(IteratorT it)
        : it_(std::move(it))
    {
        init_pointers();
    }

    int_type uflow() override final
    {
        int_type r = underflow();
        if (r != traits_type::eof()) {
            if (!gptr()) {
                BOOST_ASSERT(put_back_elems_.empty());
                put_back_elems_.pop_back();
                if (put_back_elems_.empty()) {
                    setg(eback(), backuped_pos_, egptr());
                }
            } else {
                gbump(1);
            }
        }
        return r;
    }

    int_type underflow() override final
    {
        while (egptr())
        {
            if (!gptr()) {
                BOOST_ASSERT(put_back_elems_.empty());
                return put_back_elems_.back();
            }
            if (gptr() != egptr()) {
                return traits_type::to_int_type(*gptr());
            }
            ++it_;
            init_pointers();
        }
        return traits_type::eof();
    }

    int_type pbackfail(int_type c) override final
    {
        if (traits_type::eof() != c) {
            put_back_elems_.push_back(c);
            if (gptr()) {
                backuped_pos_ = gptr();
                setg(eback(), nullptr, egptr());
            }
        }
        return c;
    }

private:
    void init_pointers()
    {
        if (!it_.empty()) {
            iterator_value_t<IteratorT> buff = *it_;
            ElemT * b = (ElemT*)(buff.begin()), *e = (ElemT*)(buff.end());
            setg(b, b, e);
        } else {
            setg(nullptr, nullptr, nullptr);
        }
    }

    IteratorT it_;
    std::vector<int_type> put_back_elems_;
    ElemT * backuped_pos_;
};

template <class IteratorT, typename ElemT, class TraitsT>
class ostreambuf : public std::basic_streambuf<ElemT, TraitsT>
{
    using base_t = std::basic_streambuf<ElemT, TraitsT>;
    using traits_type = typename base_t::traits_type;
    using int_type = typename base_t::int_type;

    using base_t::setp;
    using base_t::pbase;
    using base_t::pptr;
    using base_t::epptr;

public:
    explicit ostreambuf(IteratorT it) : it_(std::move(it))
    {
        init_pointers();
    }

    ~ostreambuf()
    {
        do_flush();
    }

    int_type overflow(int_type ic) override final
    {
        if (traits_type::eof() != ic) {
            ElemT c = traits_type::to_char_type(ic);
            *pptr() = c;
            if (pptr() == epptr()) {
                ++it_;
                init_pointers();
            }
        } else {
            do_flush();
        }

        return ic;
    }

private:
    void init_pointers()
    {
        iterator_value_t<IteratorT> buff = *it_;
        ElemT * b = (ElemT*)(buff.begin()), *e = (ElemT*)(buff.end());
        setp(b, e - 1);
    }

    void do_flush()
    {
        *it_ = array_view<ElemT>(pbase(), pptr());
        if constexpr (iterators::has_method_flush_v<IteratorT, void()>) {
            it_.flush();
        }
    }

    IteratorT it_;
};

template <class InputIteratorT, typename ElemT = remove_cv_t<iterator_value_t<InputIteratorT>>, class TraitsT = std::char_traits<ElemT>>
class istream 
    : private boost::base_from_member<istreambuf<InputIteratorT, ElemT, TraitsT>>
    , public std::basic_istream<ElemT, TraitsT>
{
    typedef istreambuf<InputIteratorT, ElemT, TraitsT> streambuf_t;
    typedef boost::base_from_member<streambuf_t> pbase_type;

public:
    explicit istream(InputIteratorT it) 
        : pbase_type(std::move(it))
        , std::istream(&this->member)
    {}
};

template <class OutputIteratorT, typename ElemT = remove_cv_t<iterator_value_t<OutputIteratorT>>, class TraitsT = std::char_traits<ElemT>>
class ostream 
    : protected boost::base_from_member<ostreambuf<OutputIteratorT, ElemT, TraitsT>>
    , public std::basic_ostream<ElemT, TraitsT>
{
    typedef ostreambuf<OutputIteratorT, ElemT, TraitsT> streambuf_t;
    typedef boost::base_from_member<streambuf_t> pbase_type;

public:
    explicit ostream(OutputIteratorT it)
        : pbase_type(std::move(it))
        , std::ostream(&this->member)
    {}
};

}}

#endif // SONIA_SERIALIZATION_IOSTREAM_HPP
