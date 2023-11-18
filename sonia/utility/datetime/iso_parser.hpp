//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include <boost/spirit/include/qi_parse.hpp>

#include "sonia/utility/parsers/utility.hpp"
#include "basic_datetime.hpp"

namespace sonia::parsers::datetime {

template <typename DerivedT, typename TagT = basic_datetime_tag<>>
struct datetime_parser_base
    : boost::spirit::qi::primitive_parser<DerivedT>
{
    using datetime_parser_base_t = datetime_parser_base;
    using datetime_tag = TagT;
    using datetime_type = typename datetime_tag::datetime_type;

    // Define the attribute type exposed by this parser component
    template <typename ContextT, typename IteratorT>
    struct attribute
    {
        using type = typename datetime_tag::datetime_type;
    };

    // This function is called during error handling to create
    // a human readable string for the error context.
    template <typename ContextT>
    boost::spirit::info what(ContextT&) const
    {
        return boost::spirit::info("datetime_parser");
    }

    // This function is called during the actual parsing process
    template <typename IteratorT, typename ContextT, typename SkipperT, typename AttributeT>
    bool parse(IteratorT& first, IteratorT const& last, ContextT& ctx, SkipperT const& skipper, AttributeT& attr) const
    {
        boost::spirit::qi::skip_over(first, last, skipper);
        if (first == last) return false;

        datetime_type result;
        if (!DerivedT::do_parse(first, last, result)) return false;
        boost::spirit::traits::assign_to(result, attr);

        return true;
    }
};

template <bool ParseTimeZoneV, typename TagT = basic_datetime_tag<>>
struct iso_parser : datetime_parser_base<iso_parser<ParseTimeZoneV>, TagT>
{
    using datetime_tag = typename iso_parser::datetime_parser_base_t::datetime_tag;
    using datetime_type = typename iso_parser::datetime_parser_base_t::datetime_type;

    template <typename IteratorT>
    static bool do_parse_time(IteratorT& first, IteratorT const& last,
        unsigned int& hour, unsigned int& minute, unsigned int& second,
        double *pfraqsecond = nullptr, int * pzsign = nullptr, int * pzhour = nullptr, int* pzminute = nullptr)
    {
        namespace sp = sonia::parsers;

        if (!sp::integer(first, last, 2, 2, hour) || hour > 23 || first == last || *first != ':') return false;
        ++first;

        if (!sp::integer(first, last, 2, 2, minute) || minute > 59 || first == last || *first != ':') return false;
        ++first;
        
        if (!sp::integer(first, last, 2, 2, second) || second > 59) return false;
        
        if (first != last && *first == '.') {
            ++first;
            int64_t partial;
            static const int maxfraqdigits = 9;
            unsigned int dcnt = sp::integer(first, last, 0, maxfraqdigits, partial);
            if (pfraqsecond) {
                for (unsigned int k = dcnt; k < maxfraqdigits; ++k, partial *= 10);
                *pfraqsecond = partial / pow(10.0, maxfraqdigits);
            }
            // skip other digits
            while (first != last && sp::digit(first));
        }
        
        if constexpr (ParseTimeZoneV) {
            if (first != last) {
                int zhour = 0, zminute = 0;
                if (*first == 'Z') ++first;
                else if (*first == '+' || *first == '-') {
                    if (pzsign) *pzsign = *first == '+' ? 1 : -1;
                    ++first;
                    if (!sp::integer(first, last, 2, 2, pzhour ? *pzhour : zhour) || first == last || *first != ':') return false;
                    ++first;
                    if (!sp::integer(first, last, 2, 2, pzminute ? *pzminute : zminute)) return false;
                }
            }
        }
        return true;
    }

    template <typename IteratorT>
    static bool do_parse(IteratorT& first, IteratorT const& last, datetime_type & result)
    {
        namespace sp = sonia::parsers;
        // parse AD/BC

        int sign = 1;
        if (*first == '+') ++first;
        else if (*first == '-') {
            sign = -1;
            ++first;
        }
        
        int64_t year = 0;
        if (!sp::integer(first, last, 4, 18, year) || first == last || *first != '-') return false;
        year *= sign;
        ++first;

        unsigned int month;
        if (!sp::integer(first, last, 2, 2, month) || first == last || *first != '-') return false;
        ++first;

        unsigned int day;
        if (!sp::integer(first, last, 2, 2, day)) return false;
        
        if (!datetime_tag::check_date(year, month, day)) return false;

        unsigned int hour = 0;
        unsigned int minute = 0;
        unsigned int second = 0;
        double fraqsecond = 0;
        int zhour = 0, zminute = 0;

        if (first != last && *first == 'T') {
            ++first;
            if (!do_parse_time(first, last, hour, minute, second, &fraqsecond, &sign, &zhour, &zminute)) return false;
        }

        datetime_type temp = datetime_tag::construct(year, month, day, hour, minute, second + fraqsecond);
        result = datetime_type(temp.ticks() - sign * (zhour * 3600 + zminute * 60) * datetime_tag::ticks_per_second);
        
        return true;
    }
};

}
