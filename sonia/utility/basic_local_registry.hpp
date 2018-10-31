//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#ifndef SONIA_BASIC_LOCAL_REGISTRY_HPP
#define SONIA_BASIC_LOCAL_REGISTRY_HPP

#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <atomic>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "sonia/string.hpp"
#include "sonia/shared_ptr.hpp"
#include "sonia/thread.hpp"
#include "sonia/utility/persister.hpp"
#include "sonia/utility/thread/rw_fiber_mutex.hpp"
#include "sonia/utility/scope_exit.hpp"

namespace sonia { namespace utility { namespace basic_local_registry_detail {

struct persisted_flag {
    mutable std::atomic<bool> persisted;
    persisted_flag() : persisted(false) {}
    persisted_flag(persisted_flag const& rhs) : persisted(rhs.persisted.load()) {}
    persisted_flag & operator=(persisted_flag const& rhs) {
        persisted.store(rhs.persisted.load());
        return *this;
    }
};

template <typename IDT>
struct reg_item : persisted_flag {
    std::string name, meta;
    IDT id;

    reg_item(std::string nm, IDT idval, std::string mi = "") : name(std::move(nm)), meta(std::move(mi)), id(idval) {}

    reg_item() = default;
    reg_item(reg_item const& rhs) = default;
    reg_item(reg_item && rhs) = default;
    reg_item & operator=(reg_item const&) = default;
    reg_item & operator=(reg_item &&) = default;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int = 0) {
        ar & boost::serialization::make_nvp("name", name)
           & boost::serialization::make_nvp("id", id)
           & boost::serialization::make_nvp("meta", meta);
        if constexpr (Archive::is_loading::value) {
            persisted.store(true);
        }
    }
};

}

template <class DerivedT, typename IDT>
class basic_local_registry
{
    friend class boost::serialization::access;
    typedef basic_local_registry_detail::reg_item<IDT> reg_item;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int = 0) {
        ar & boost::serialization::make_nvp("counter", counter_)
           & boost::serialization::make_nvp("catalog", registry_);
    }

    typedef boost::multi_index::multi_index_container<
        reg_item,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::member<reg_item, std::string, &reg_item::name>,
                string_hasher
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::member<reg_item, IDT, &reg_item::id>
            >
        >
    > registry_t;

    void bootstrap() {
        counter_ = 0;
    }

    IDT increment_fetch_counter() {
        return ++counter_;
    }

    DerivedT & derived() { return static_cast<DerivedT&>(*this); }
    DerivedT const& derived() const { return static_cast<DerivedT const&>(*this); }

protected:
    void backup() const {
        state_persister_->write_stream([this](std::ostream & os) {
            boost::archive::xml_oarchive oa(os);
            const_cast<DerivedT&>(derived()).serialize(oa);
        });
    }

    void restore() {
        if (!state_persister_->read_stream([this](std::istream & is) {
            boost::archive::xml_iarchive ia(is);
            derived().serialize(ia);
        })) {
            derived().bootstrap();
        }
    }

public:
    explicit basic_local_registry(shared_ptr<persister> sp)
        : state_persister_(std::move(sp))
    {}

    IDT get_id(string_view name, string_view meta) {
        auto rwguard = make_rw_lock_guard(mtx_, rw_type::shared);
        auto it = registry_.find(name, string_hasher(), string_equal_to());
        reg_item const* pitm = it != registry_.end() ? &*it : nullptr;
        
        if (BOOST_UNLIKELY(!pitm)) {
            rwguard.promote();
            // "it" could be invalidated (by other threads), so get it again and check
            it = registry_.find(name, string_hasher(), string_equal_to());
            if (it == registry_.end()) {
                IDT result = derived().increment_fetch_counter();
                pitm = &*registry_.insert(it, reg_item{to_string(name), result, to_string(meta)});
            } else {
                pitm = &*it;
            }
            rwguard.demote();
        }

        if (BOOST_UNLIKELY(!pitm->persisted.load())) {
            auto bguard = make_lock_guard(backup_mtx_);
            if (!pitm->persisted.load()) {
                backup();
                for (auto & itm : registry_) {
                    itm.persisted.store(true);
                }
            }
            BOOST_ASSERT(pitm->persisted.load());
        }
        
        return pitm->id;
    }

    string_view get_name(IDT id) const {
        auto guard = make_shared_lock_guard(mtx_);
        auto it = registry_.template get<1>().find(id);
        if (it != registry_.template get<1>().end() && it->persisted.load()) {
            return it->name;
        }
        throw internal_error("no registry item with id: %1%"_fmt % id);
    }

private:
    IDT counter_;
    shared_ptr<persister> state_persister_;
    registry_t registry_;
    mutable fibers::rw_mutex mtx_;
    mutable fibers::mutex backup_mtx_;
};

}}

#endif // SONIA_BASIC_LOCAL_REGISTRY_HPP
