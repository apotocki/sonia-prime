//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#pragma once

#include <vector>
#include <cstdlib>

#include "sonia/exceptions.hpp"
#include "sonia/utility/parsers/utility.hpp"

namespace sonia::parsers::xml {

template <typename DerivedT>
class model_base
{
    model_base(model_base const&) = delete;
    model_base& operator= (model_base const&) = delete;

    DerivedT const& derived() const { return static_cast<DerivedT const&>(*this); }
    DerivedT & derived() { return static_cast<DerivedT&>(*this); }

public:
    model_base() = default;
    //~model_base(){}

    // XML Decl
    void on_encoding() {}
    void on_version() {}
    void on_standalone()
    {
        string_view sv = array_view(tempstr_);
        if (sv == "yes") derived().on_standalone(true);
        else if (sv == "no") derived().on_standalone(false);
        else throw exception("wrong standalone value %1%"_fmt % sv);
    }
    void on_standalone(bool){}

    template <typename IteratorT> void on_pi(IteratorT b, IteratorT e)
    {
        put_string(b, e);
    }

    void on_start_doctype() {}
    void on_system_literal() {}
    void on_pubid_literal() {}

    void on_begin_element() { element_name_ = std::move(tempstr_); }
    void on_end_element() { } // on '>'
    void on_close_element() { } // on '/>'
    void on_close_tag() { } // on '</' name
    void on_attribute_name() { attr_name_ = std::move(tempstr_); }
    void on_attribute_value() {}
    void on_char_data() {}

    template <typename IteratorT>
    void put_string(IteratorT b, IteratorT e, uint8_t qsz = 1)
    {
        for (uint8_t v = qsz; v > 0; ++b, --v);
        tempstr_.clear();
        std::copy(b, e, std::back_inserter(tempstr_));
        //normilize_xml_string(b, e, std::back_inserter(tempstr_));
        BOOST_ASSERT(qsz <= tempstr_.size());
        tempstr_.resize(tempstr_.size() - qsz);
    }

    template <typename IteratorT>
    void put_cdata_string(IteratorT b, IteratorT e)
    {
        for (uint8_t v = 9; v > 0; ++b, --v);
        tempstr_.clear();
        std::copy(b, e, std::back_inserter(tempstr_));
        tempstr_.resize(tempstr_.size() - 3);
    }

protected:
    std::vector<char> tempstr_, element_name_, attr_name_;
};

class basic_model : public model_base<basic_model>
{

};

}
