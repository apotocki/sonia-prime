//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
//#include "sonia/utility/xmlbuilder/advxmlbuilder.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

namespace sonia::xmlbuilder::detail {

template <typename IteratorT, typename TypeFactoryT>
class type_parser_grammar 
    : public boost::spirit::qi::grammar<IteratorT, typename TypeFactoryT::type(), boost::spirit::ascii::space_type>
    , TypeFactoryT
{
    using type_t = typename TypeFactoryT::type;

public:
    template <typename ... ArgsT>
    type_parser_grammar(ArgsT && ... args) 
        : type_parser_grammar::base_type(type)
        , TypeFactoryT(std::forward<ArgsT>(args)...)
    {
        using namespace boost::spirit;
        using qi::lexeme;
        using qi::uint_;
        using ascii::alpha;
        using boost::phoenix::push_back;
        using qi::_1;

        type_name = lexeme[+alpha[_val += _1]] || qi::string("{}")[_val += _1];
        type = ((type_name[_val = boost::phoenix::bind(&TypeFactoryT::create, static_cast<TypeFactoryT*>(this), _1)] >> -('[' >> uint_ >> ']')[boost::phoenix::bind(&arrayfy, _val, _1)])) ||
            ('(' >> (type % ',') >> ')')[boost::phoenix::bind(&tuplefy, _val, _1)]
        ;
        //type = (-('[' << uint_ << ']'))[_val = boost::phoenix::back(boost::phoenix::ref(stack))];
        //type = type_name[push_back(boost::phoenix::ref(stack), _1)] << -('[' << uint_ << ']');
    
    
    
    }

//protected:

    static void arrayfy(type_t & tp, size_t sz)
    {
        tp = x_array<type_t>(std::move(tp), (size_t)sz);
    }

    static void tuplefy(type_t& tp, std::vector<type_t> & vec)
    {
        tp = x_tuple<type_t>{vec};
    }

private:
    boost::spirit::qi::rule<IteratorT, type_t(), boost::spirit::ascii::space_type> type;
    boost::spirit::qi::rule<IteratorT, std::string(), boost::spirit::ascii::space_type> type_name;

    //std::vector<TypeT> stack;
    std::vector<std::string> stack;
};

}
