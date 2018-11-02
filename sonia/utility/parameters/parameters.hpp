//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_UTILITY_PARAMETERS_HPP
#define SONIA_UTILITY_PARAMETERS_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <typeinfo>

#include <boost/unordered_set.hpp>

#include "sonia/shared_ptr.hpp"
#include "sonia/function.hpp"
#include "sonia/optional.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/json/value.hpp"
#include "sonia/utility/json/json_cast.hpp"
#include "sonia/utility/functional/hash/string.hpp"

namespace sonia { namespace parameters {

template <class BoundT> class parameters_description;
template <class VDT> class parameter_options;

class value_descriptor {
public:
    explicit value_descriptor(const char* name)
        : name_(name), required_(false)
    {}

    virtual ~value_descriptor() {}

    value_descriptor() : required_(false) {}

    void set_name(std::string val) { name_ = std::move(val); }
    void set_description(std::string val) { descr_ = std::move(val); }
    void set_required(bool v = true) { required_ = v; }
    void set_default_json_string(std::string const&);
    virtual void set_default_json(json_value) = 0;
    
    std::string const& name() const { return name_; }
    std::string const& description() const { return descr_; }
    bool is_required() const { return required_; }
    virtual bool has_default() const = 0;

    virtual void assign(json_value const&, std::type_info const&, void*) const = 0;
    virtual void assign_default(std::type_info const&, void*) const = 0;

protected:
    std::string name_, descr_;
    bool required_;
};

template <typename T, typename BoundT>
class typed_value_descriptor : public value_descriptor {
public:
    typedef T target_type;
    typedef BoundT bound_type;
    typedef parameters_description<bound_type> parameters_description_type;

    typed_value_descriptor(const char * name, T BoundT::* pm)
        : value_descriptor(name), pmember_(pm)
    {}

    void set_default_value(T && val) {
        default_ = std::move(val);
    }

    void set_default_json(json_value val) override final {
        T result;
        assign(val, typeid(T), &result);
        default_ = std::move(result);
    }

    bool has_default() const override final {
        return !!default_;
    }

    void assign_default(std::type_info const& ti, void* obj) const override final {
        member(ti, obj) = *default_;
    }

protected:
    T& member(std::type_info const& ti, void* obj) const {
        if (ti == typeid(BoundT)) {
            return reinterpret_cast<BoundT*>(obj)->*pmember_;
        } else {
            throw internal_error("attempt to use an incompatible type %1% instead of %2%"_fmt % ti.name() % typeid(BoundT).name());
        }
    }

private:
    T BoundT::* pmember_;
    optional<T> default_;
};

template <typename T, typename BoundT>
class simple_value_descriptor : public typed_value_descriptor<T, BoundT> {
    typedef typed_value_descriptor<T, BoundT> base_t;

public:
    typedef parameters_description<T> binder_type;

    simple_value_descriptor(const char * name, T BoundT::* pm) : base_t(name, pm)
    {}

    ~simple_value_descriptor() override {}

    void assign(json_value const& val, std::type_info const& ti, void* obj) const override final {
        base_t::member(ti, obj) = cast(val);
    }

    void set_binder(shared_ptr<binder_type> b) { binder_ = std::move(b); }
    void set_jh(function<T(json_value const& v)> const& jh) { jh_ = jh; }

private:
    T cast(json_value const& v) const;

    shared_ptr<binder_type> binder_;
    function<T(json_value const& v)> jh_;
};

template <typename T, typename BoundT>
class collection_value_descriptor : public typed_value_descriptor<T, BoundT> {
    typedef typed_value_descriptor<T, BoundT> base_t;
    typedef range_value_t<T> item_t;

public:
    typedef parameters_description<item_t> binder_type;

    collection_value_descriptor(const char * name, T BoundT::* pm) : base_t(name, pm)
    {}

    ~collection_value_descriptor() override {}

    void assign(json_value const& val, std::type_info const& ti, void* obj) const override final {
        auto & coll = base_t::member(ti, obj);
        if (json_value_type::array == val.type()) {
            for (json_value const& e : val.get_array()) {
                try {
                    coll.push_back(cast(e));
                } catch (std::exception const& ex) {
                    throw exception("Error occurred during pushing value %1% into collection\n%2%"_fmt % to_string(e) % ex.what());
                }
            }
        } else {
            coll.push_back(cast(val));
        }    
    }

    void set_binder(shared_ptr<binder_type> b) { binder_ = std::move(b); }

private:
    item_t cast(json_value const& v) const;

    shared_ptr<binder_type> binder_;
};

template <typename T, typename BoundT>
class map_value_descriptor : public typed_value_descriptor<T, BoundT> {
    typedef typed_value_descriptor<T, BoundT> base_t;
    typedef typename T::mapped_type item_t;

public:
    typedef parameters_description<item_t> binder_type;

    map_value_descriptor(const char * name, T BoundT::* pm) : base_t(name, pm)
    {}

    ~map_value_descriptor() override {}

    void assign(json_value const& val, std::type_info const& ti, void* obj) const override final {
        auto & coll = base_t::member(ti, obj);
        json_object jo = val.get_object();
        for (auto const& pair : jo.items()) {
            try {
                if (!coll.insert(std::make_pair(to_string(pair.first), cast(pair.second))).second) {
                    throw exception("Named item '%1%' is already inserted, previous value: %2%"_fmt % pair.first % to_string(*jo[pair.first]));
                }
            } catch (std::exception const& ex) {
                throw exception("Error occurred during inserting named value '%1%' into collection\n%2%"_fmt % pair.first % ex.what());
            }
        }
    }

    void set_binder(shared_ptr<binder_type> b) { binder_ = std::move(b); }

private:
    item_t cast(json_value const& v) const;

    shared_ptr<binder_type> binder_;
};

template <class BoundT>
class parameters_binding {
    template <class> friend class parameters_description;

protected:
    explicit parameters_binding(parameters_description<BoundT> * pd) : pd_(pd) {}

public:
    template <typename ET>
    parameter_options<simple_value_descriptor<ET, BoundT>> variable(const char * name, ET BoundT::* elem, const char * descr = nullptr);

    template <typename ET>
    parameter_options<collection_value_descriptor<ET, BoundT>> array(const char * name, ET BoundT::* elem, const char * descr = nullptr);

    template <typename ET>
    parameter_options<map_value_descriptor<ET, BoundT>> map(const char * name, ET BoundT::* elem, const char * descr = nullptr);

    parameters_description<BoundT> & description() const { return *pd_; }

private:
    parameters_description<BoundT> * pd_;
};

template <class VDT>
class parameter_options : public parameters_binding<typename VDT::bound_type>
{
    template <class> friend class parameters_binding;
    typedef typename VDT::bound_type bound_type;
    typedef typename VDT::target_type target_type;
    typedef typename VDT::parameters_description_type parameters_description_type;
    typedef typename VDT::binder_type binder_type;

    parameter_options(parameters_description_type * pd, VDT * vd) 
        : parameters_binding<bound_type>(pd), vd_(vd)
    {}

public:
    parameter_options & required(bool v = true) {
        vd_->set_required(v);
        return *this;
    }

    parameter_options & default_value(target_type & val) {
        vd_->set_default_value(T(val));
        return *this;
    }

    parameter_options & default_value(target_type const& val) {
        vd_->set_default_value(T(val));
        return *this;
    }

    parameter_options & default_value(target_type && val) {
        vd_->set_default_value(std::move(val));
        return *this;
    }

    parameter_options & default_json_value(std::string const& json) {
        vd_->set_default_json_string(json);
        return *this;
    }

    parameter_options & binder(binder_type const& pd);

    parameter_options & binder(function<target_type(json_value const& v)> const& jh);

    template <typename NextBoundT>
    parameter_options & binder(parameters_binding<NextBoundT>);

private:
    VDT * vd_;
};

template <class BoundT>
class parameters_description {
    typedef std::vector<shared_ptr<value_descriptor>> descriptors_t;

public:
    parameters_description()
        : vds_(make_shared<descriptors_t>())
    {}

    descriptors_t & descriptors() { return *vds_; }

    parameters_binding<BoundT> bind() { return parameters_binding<BoundT>(this); }

    void apply(json_object const& jo, BoundT * obj);

private:
    shared_ptr<descriptors_t> vds_;
};

template <typename T, typename BoundT>
T simple_value_descriptor<T, BoundT>::cast(json_value const& v) const {
    if (binder_) {
        T val{};
        binder_->apply(v.get_object(), &val);
        return std::move(val);
    } else if (jh_) {
        return jh_(v);
    } else {
        return json_cast<T>(v);
    }
}

template <typename T, typename BoundT>
typename collection_value_descriptor<T, BoundT>::item_t collection_value_descriptor<T, BoundT>::cast(json_value const& v) const {
    if (binder_) {
        item_t val{};
        binder_->apply(v.get_object(), &val);
        return std::move(val);
    } else {
        return json_cast<item_t>(v);
    }
}

template <typename T, typename BoundT>
typename map_value_descriptor<T, BoundT>::item_t map_value_descriptor<T, BoundT>::cast(json_value const& v) const {
    if (binder_) {
        item_t val{};
        binder_->apply(v.get_object(), &val);
        return std::move(val);
    } else {
        return json_cast<item_t>(v);
    }
}

template <class BoundT>
template <typename ET>
parameter_options<simple_value_descriptor<ET, BoundT>> parameters_binding<BoundT>::variable(const char * name, ET BoundT::* var, const char * descr) {
    auto vd = make_shared<simple_value_descriptor<ET, BoundT>>(name, var);
    if (descr) vd->set_description(descr);
    pd_->descriptors().push_back(vd);
    return parameter_options<simple_value_descriptor<ET, BoundT>>(pd_, vd.get());
}

template <class BoundT>
template <typename ET>
parameter_options<collection_value_descriptor<ET, BoundT>> parameters_binding<BoundT>::array(const char * name, ET BoundT::* elem, const char * descr) {
    auto vd = make_shared<collection_value_descriptor<ET, BoundT>>(name, elem);
    if (descr) vd->set_description(descr);
    pd_->descriptors().push_back(vd);
    return parameter_options<collection_value_descriptor<ET, BoundT>>(pd_, vd.get());
}

template <class BoundT>
template <typename ET>
parameter_options<map_value_descriptor<ET, BoundT>> parameters_binding<BoundT>::map(const char * name, ET BoundT::* elem, const char * descr) {
    auto vd = make_shared<map_value_descriptor<ET, BoundT>>(name, elem);
    if (descr) vd->set_description(descr);
    pd_->descriptors().push_back(vd);
    return parameter_options<map_value_descriptor<ET, BoundT>>(pd_, vd.get());
}

template <class VDT>
parameter_options<VDT> & parameter_options<VDT>::binder(binder_type const& pd) {
    vd_->set_binder(make_shared<binder_type>(pd));
    return *this;
}

template <class VDT>
parameter_options<VDT> & parameter_options<VDT>::binder(function<typename VDT::target_type(json_value const& v)> const& jh) {
    vd_->set_jh(jh);
    return *this;
}

template <class VDT>
template <typename NextBoundT>
parameter_options<VDT> & parameter_options<VDT>::binder(parameters_binding<NextBoundT> pb) {
    vd_->set_binder(make_shared<binder_type>(pb.description()));
    return *this;
}

template <class BoundT>
void parameters_description<BoundT>::apply(json_object const& jo, BoundT * obj) {
    try {
        boost::unordered_set<std::string, hasher> used_names;
        for (auto const& pd : descriptors()) {
            json_value const* val = jo[pd->name()];
            if (!val && !pd->is_required() && pd->has_default()) {
                pd->assign_default(typeid(BoundT), obj);
            } else if (!val && pd->is_required()) {
                throw exception("Property '%1%' is required but not provided"_fmt % pd->name());
            } else if (val) {
                used_names.insert(pd->name());
                try {
                    pd->assign(*val, typeid(BoundT), obj);
                } catch (std::exception const& e) {
                    throw exception("Error occurred during applying value %1% for %2%\n%3%"_fmt % to_string(*val) % pd->name() % e.what());
                }
            }
        }
        for (auto const& item : jo.items()) {
            if (used_names.find(item.first, hasher(), string_equal_to()) == used_names.cend()) {
                throw exception("An unbound parameter '%1%' was found"_fmt % item.first);
            }
        }
    } catch (std::exception const& e) {
        throw exception("Error occurred during applying parameters for %1%\n%2%"_fmt % typeid(BoundT).name() % e.what());
    }
}

}}

#endif // SONIA_UTILITY_PARAMETERS_HPP
