//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once
#include "sonia/utility/mvvm/view_model.hpp"
#include "sonia/utility/invocation/invocation.hpp"
#include "sonia/bang/bang.hpp"

namespace sonia {

// it's abstract class, still virtual: get_manager, set(manager), self_as_content_view_shared
class bang_view_model
    : public virtual view_model
    , public sonia::lang::bang::language
    , public invocation::registrar<bang_view_model, view_model>
{
    using registrar_type = invocation::registrar<bang_view_model, view_model>::registrar_type;
    friend class invocation::registrar<bang_view_model, view_model>;

public:
    using view_model::view_model;

    // scripting
    void load_file(string_view path);

protected:
    // methods routine
    static void do_registration(registrar_type& mr);
};

//class bang_canvas_view_model
//    : public bang_view_model
//    , public enable_shared_from_this<bang_canvas_view_model>
//    , public invocation::registrar<bang_canvas_view_model, bang_view_model>
//{
//public:
//    explicit bang_canvas_view_model(int32_t idval, shared_ptr<manager> mng = {})
//        : bang_view_model{ idval }, mng_{ std::move(mng) }
//    {}
//
//    shared_ptr<manager> get_manager() const override final { return mng_.lock(); }
//    void set(shared_ptr<manager> mng) override final { mng_ = mng; }
//
//    shared_ptr<view_model> self_as_content_view_shared() override final { return shared_from_this(); }
//
//    template <typename RT>
//    static void do_registration(RT& mr)
//    {
//        mr.inherit(typeid(bang_view_model));
//    }
//
//private:
//    weak_ptr<manager> mng_;
//};

}
