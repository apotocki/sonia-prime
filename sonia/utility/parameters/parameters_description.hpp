//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_PARAMETERS_DESCRIPTION_HPP
#define SONIA_PARAMETERS_DESCRIPTION_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <set>

#include "sonia/shared_ptr.hpp"
#include "sonia/function.hpp"
#include "sonia/optional.hpp"
#include "sonia/utility/json/value.hpp"

namespace sonia { namespace parameters {

class parameters_description_base;
template <class BoundT> class parameters_description;

class value_descriptor {
public:
    explicit value_descriptor(const char* name)
        : name_(name)
    {}

    virtual ~value_descriptor() {}

    value_descriptor() : required_(false) {}

    void set_binder(shared_ptr<parameters_description_base> b) { binder_ = std::move(b); }
    void set_name(std::string val) { name_ = std::move(val); }
    void set_description(std::string val) { descr_ = std::move(val); }
    void set_required(bool v = true) { required_ = v; }
    void set_default_json_string(std::string const&);
    virtual void set_default_json(json_value) = 0;
    
    std::string const& name() const { return name_; }
    std::string const& description() const { return descr_; }
    bool is_required() const { return required_; }
    virtual bool has_default() const = 0;

    virtual void assign_default(void*) const = 0;

protected:
    shared_ptr<parameters_description_base> binder_;
    std::string name_, descr_;
    bool required_;
};

template <typename T, typename BoundT>
class typed_value_descriptor : public value_descriptor {
public:
    typed_value_descriptor(const char * name, T BoundT::* pm)
        : value_descriptor(name), pmember_(pm)
    {}

    ~typed_value_descriptor() override {}

    void set_default_value(T && val) {
        default_ = std::move(val);
    }

    bool has_default() const override final {
        return !!default_;
    }
    
    virtual void assign_default(void* obj) const {
        reinterpret_cast<BoundT*>(obj)->*pmember_ = *default_;
    }
    

    void set_default_json(json_value) {
        
    }

private:
    T BoundT::* pmember_;
    optional<T> default_;
};

class parameters_description_base {
    typedef std::vector<shared_ptr<value_descriptor>> descriptors_t;

public:
    parameters_description_base()
        : vds_(make_shared<descriptors_t>())
    {}

    descriptors_t & descriptors() { return *vds_; }

private:
    shared_ptr<descriptors_t> vds_;
};

template <typename T, class BoundT> class parameter_options;

template <class BoundT>
class parameters_binding {
    template <class> friend class parameters_description;

protected:
    explicit parameters_binding(parameters_description<BoundT> * pd) : pd_(pd) {}

public:
    //template <typename ET>
    //parameters_binding & operator()(const char * name, ET BT::* elem) {
    //    std::cout << typeid(ET).name() << "\n";
    //    return *this;
    //}

    template <typename ET>
    parameter_options<ET, BoundT> variable(const char * name, ET BoundT::* elem, const char * descr = nullptr);

    template <typename ET>
    parameter_options<ET, BoundT> array(const char * name, ET BoundT::* elem, const char * descr = nullptr);

    parameters_description<BoundT> & description() const { return *pd_; }

private:
    parameters_description<BoundT> * pd_;
};

template <typename T, class BoundT>
class parameter_options : public parameters_binding<BoundT>
{
    template <class> friend class parameters_binding;
    //template <class, class> friend class parameter_options;

    parameter_options(parameters_description<BoundT> * pd, typed_value_descriptor<T, BoundT> * vd) 
        : parameters_binding<BoundT>(pd), vd_(vd)
    {}

public:
    parameter_options & required(bool v = true) {
        vd_->set_required(v);
        return *this;
    }

    parameter_options & default_value(T & val) {
        vd_->set_default_value(T(val));
        return *this;
    }

    parameter_options & default_value(T const& val) {
        vd_->set_default_value(T(val));
        return *this;
    }

    parameter_options & default_value(T && val) {
        vd_->set_default_value(std::move(val));
        return *this;
    }

    parameter_options & default_json_value(std::string const& json) {
        vd_->set_default_json_string(json);
        return *this;
    }

    template <typename NextBoundT>
    parameter_options & binder(parameters_description<NextBoundT> const& pd);

    template <typename NextBoundT>
    parameter_options & binder(parameters_binding<NextBoundT>);

private:
    typed_value_descriptor<T, BoundT> * vd_;
};



template <class BoundT>
class parameters_description : public parameters_description_base {
public:
    parameters_binding<BoundT> bind() { return parameters_binding<BoundT>(this); }

    void apply(json_object jo, BoundT * obj);
    //parameters_description_init add_parameters();

    //void for_each(function<void(value_descriptor const&)> const& ftor) const;

};

template <class BoundT>
template <typename ET>
parameter_options<ET, BoundT> parameters_binding<BoundT>::variable(const char * name, ET BoundT::* var, const char * descr) {
    auto vd = make_shared<typed_value_descriptor<ET, BoundT>>(name, var);
    if (descr) vd->set_description(descr);
    pd_->descriptors().push_back(vd);

    //std::cout << typeid(range_value_t<ET>).name() << "\n";
    return parameter_options<ET, BoundT>(pd_, vd.get());
}

template <class BoundT>
template <typename ET>
parameter_options<ET, BoundT> parameters_binding<BoundT>::array(const char * name, ET BoundT::* elem, const char * descr) {
    auto vd = make_shared<typed_value_descriptor<ET, BoundT>>(name, elem);
    if (descr) vd->set_description(descr);
    pd_->descriptors().push_back(vd);

    //std::cout << typeid(range_value_t<ET>).name() << "\n";
    return parameter_options<ET, BoundT>(pd_, vd.get());
}

template <typename ET, class BoundT>
template <typename NextBoundT>
parameter_options<ET, BoundT> & parameter_options<ET, BoundT>::binder(parameters_description<NextBoundT> const& pd) {
    vd_->set_binder(make_shared<parameters_description<NextBoundT>>(pd));
    return *this;
}

template <typename ET, class BoundT>
template <typename NextBoundT>
parameter_options<ET, BoundT> & parameter_options<ET, BoundT>::binder(parameters_binding<NextBoundT> pb) {
    vd_->set_binder(make_shared<parameters_description<NextBoundT>>(pb.description()));
    return *this;
}

template <class BoundT>
void parameters_description<BoundT>::apply(json_object jo, BoundT * obj) {
    std::set<std::string> used_names;
    for (auto const& pd : descriptors()) {
        json_value const* val = jo[pd->name()];
        if (!val && !pd->is_required() && pd->has_default()) {
            pd->assign_default(obj);
        } else if (!val && pd->is_required()) {
            throw exception("The property '%1%' is required but not provided"_fmt % pd->name());
        }
    }
}

}}

#endif // SONIA_PARAMETERS_DESCRIPTION_HPP
