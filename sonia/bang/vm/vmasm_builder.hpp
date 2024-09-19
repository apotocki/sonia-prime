//  Sonia.one framework(c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <boost/intrusive/list.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "sonia/variant.hpp"

#include "sonia/utility/lang/vm.hpp"

#include "sonia/utility/automatic_polymorphic.hpp"
#include "sonia/utility/object_pool.hpp"

#define VM_INSTRUCTION_ENTRIED_POOL_SIZE 128

namespace sonia::vmasm {

using namespace sonia::vm;

class function_identity : public polymorphic_clonable_and_movable
{
public:
    virtual bool equal(function_identity const& rhs) const noexcept = 0;
    virtual size_t hash() const noexcept = 0;

    inline friend bool operator==(function_identity const& l, function_identity const& r) noexcept { return l.equal(r); }
    inline friend size_t hash_value(function_identity const& v) noexcept { return v.hash(); }
};

template <typename IDT>
class fn_identity : public function_identity
{
    IDT id_;

public:
    inline explicit fn_identity(IDT id) noexcept : id_{ std::move(id) } {}

    SONIA_POLYMORPHIC_CLONABLE_MOVABLE_IMPL(fn_identity)

    bool equal(function_identity const& rhs) const noexcept override
    {
        if (auto *pfnid = dynamic_cast<fn_identity const*>(&rhs); pfnid) {
            return id_ == pfnid->id_;
        }
        return false;
    }

    size_t hash() const noexcept { return hash_value(id_); }
};

template <typename ContextT>
class builder
{
    using variable_type = typename ContextT::variable_type;

public:
    using vm_t = virtual_stack_machine<ContextT>;
    using op_t = typename vm_t::op;

    using function_identity_store_t = automatic_polymorphic<function_identity, 2 * sizeof(void*)>;

    struct instruction_entry;
    using operand_t = variant<null_t, size_t, instruction_entry*, function_identity_store_t>;
    struct instruction_entry
    {
        boost::intrusive::list_member_hook<> hook;
        operand_t operand;
        vm_t::op operation;
    };

    using instruction_list_type = boost::intrusive::list<
        instruction_entry,
        boost::intrusive::member_hook<instruction_entry, boost::intrusive::list_member_hook<>, &instruction_entry::hook>>;

    struct function_descriptor
    {
        function_identity_store_t id;
        optional<size_t> address;
        optional<size_t> index; // index of a const used to store real fn address
        instruction_list_type instructions;

        inline explicit function_descriptor(function_identity const& idval) noexcept : id{ idval } {}

        inline friend bool operator==(function_descriptor const& l, function_descriptor const& r) noexcept { return *l.id == *r.id; }
        inline friend size_t hash_value(function_descriptor const& v) noexcept { return v.id->hash(); }

        void free(builder & b)
        {
            while (!instructions.empty()) {
                auto& i = instructions.back();
                instructions.pop_back();
                b.free_entry(i);
            }
        }
    };

    struct fd_equal
    {
        inline bool operator()(function_descriptor const& l, function_identity const& r) const noexcept { return *l.id == r; }
        inline bool operator()(function_identity const& l, function_descriptor const& r) const noexcept { return l == *r.id; }
    };

    class function_builder
    {
        builder& builder_;
        function_descriptor& dr_;

        struct block
        {
            boost::container::small_vector<uint8_t, 16> code;
            operand_t operand;
            vm_t::op operation;

            inline void append(op_t op) { code.push_back(static_cast<uint8_t>(op)); }
            inline void append(op_t op, size_t param) { append(op); append_uint(param); }
            void append_uint(size_t);
        };
        
    public:
        inline explicit function_builder(builder& b, function_descriptor& fd) noexcept
            : builder_{ b }, dr_{ fd }
        {}

        inline instruction_entry* position() { return dr_.instructions.empty() ? nullptr : &dr_.instructions.back(); }

        void append_push_pooled_const(variable_type&&);

        void append_op(op_t op, operand_t arg = {});

        inline void append_noop() { append_op(op_t::noop); }

        inline void append_push(size_t num) { append_op(op_t::push, num); }
        inline void append_pushc(size_t num) { append_op(op_t::pushc, num); }
        inline void append_pushr(size_t offset) { append_op(op_t::pushr, offset); }
        inline void append_pushi(size_t num) { append_op(op_t::pushi, num); }
        inline void append_pushfp() { append_op(op_t::pushfp); }
        inline void append_fpush(intptr_t fpos) { fpos >= 0 ? append_op(op_t::fppush, static_cast<size_t>(fpos)) : append_op(op_t::fnpush, static_cast<size_t>(-fpos)); }
        inline void append_fpushi(intptr_t fpos) { fpos >= 0 ? append_op(op_t::fppushi, static_cast<size_t>(fpos)) : append_op(op_t::fnpushi, static_cast<size_t>(-fpos)); }
        
        inline void append_pop(size_t num = 1) { append_op(op_t::popn, num); }
        inline void append_popfp() { append_op(op_t::popfp); }
        inline void append_truncatefp(intptr_t offset) { offset >= 0 ? append_op(op_t::truncatefpp, static_cast<size_t>(offset)) : append_op(op_t::truncatefpn, static_cast<size_t>(-offset)); }

        inline void append_set(size_t num) { append_op(op_t::set, num); }
        inline void append_setr(size_t offset) { append_op(op_t::setr, offset); }
        inline void append_fset(intptr_t fpos) { fpos >= 0 ? append_op(op_t::fpset, static_cast<size_t>(fpos)) : append_op(op_t::fnset, static_cast<size_t>(-fpos)); }
        
        inline void append_call(function_identity const& fnident) { append_op(op_t::call, fnident); }
        //inline void append_call(size_t address) { append_op(op_t::call, address); }
        inline void append_ecall(size_t fnindex) { append_op(op_t::ecall, fnindex); }
        inline void append_callp() { append_op(op_t::callp); }
        inline void append_ret() { append_op(op_t::ret); }

        void append_collapse(size_t num) { append_op(op_t::collapse, num); }

        inline void append_jmp(instruction_entry* ebefore) { append_op(op_t::jmp, ebefore); }

        void remove(instruction_entry* e)
        {
            dr_.instructions.erase(dr_.instructions.s_iterator_to(*e));
            builder_.free_entry(*e);
        }

        void materialize();
    };

public:
    explicit builder(vm_t& vm)
        : vm_{ vm }
        , entries_{VM_INSTRUCTION_ENTRIED_POOL_SIZE} {}

    ~builder();

    function_descriptor& resolve_function(function_identity const&);
    function_builder create_function(function_identity const&);
    size_t add_pooled_const(variable_type&&);

    inline instruction_entry& new_entry() { return *entries_.new_object(); }
    inline void free_entry(instruction_entry& e) noexcept { entries_.delete_object(&e); }

    inline size_t allocate_constant_index() { return vm_.add_const(smart_blob{}); }

private:
    vm_t & vm_;

    boost::unordered_set<function_descriptor, hasher> fns_;

    object_pool<instruction_entry, spin_mutex> entries_;

    boost::unordered_map<blob_result, size_t> literals_;
};

template <typename ContextT>
builder<ContextT>::~builder()
{
    for (auto& fn : fns_) {
        const_cast<function_descriptor&>(fn).free(*this);
    }
    fns_.clear();
}

template <typename ContextT>
typename builder<ContextT>::function_descriptor& builder<ContextT>::resolve_function(function_identity const& id)
{
    auto it = fns_.find(id, hasher{}, fd_equal{});
    if (it == fns_.end()) it = fns_.emplace_hint(it, id);
    return const_cast<function_descriptor&>(*it);
}

template <typename ContextT>
typename builder<ContextT>::function_builder builder<ContextT>::create_function(function_identity const& id)
{
    auto it = fns_.find(id, hasher{}, fd_equal{});
    if (it != fns_.end()) {
        throw exception("function with the specified identity already exists");
    }
    it = fns_.emplace_hint(it, id);
    return function_builder{*this, const_cast<function_descriptor&>(*it)};
}

template <typename ContextT>
size_t builder<ContextT>::add_pooled_const(variable_type&& value)
{
    auto it = literals_.find(*value);
    if (it == literals_.end()) {
        size_t index = vm_.add_const(std::move(value));
        it = literals_.emplace_hint(it, *vm_.consts()[index], index);
        GLOBAL_LOG_DEBUG() << "new const value: " << vm_.consts()[index] << " -> C["sv << index << "]"sv;
    } else {
        GLOBAL_LOG_DEBUG() << "found value: " << value << " at C[" << it->second << "]"sv;
    }
    return it->second;
}

template <typename ContextT>
void builder<ContextT>::function_builder::append_op(op_t op, operand_t arg)
{
    instruction_entry& e = builder_.new_entry();
    e.operation = op;
    e.operand = std::move(arg);
    dr_.instructions.push_back(e);
}

template <typename ContextT>
void builder<ContextT>::function_builder::append_push_pooled_const(variable_type&& value)
{
    size_t pos = builder_.add_pooled_const(std::move(value));
    append_pushc(pos);
}

template <typename ContextT>
void builder<ContextT>::function_builder::block::append_uint(size_t uval)
{
    uint8_t b = static_cast<uint8_t>(uval & 0x7f);
    bool has_more = !!(uval - b);
    code.push_back(b);
    if (!has_more) return;
    size_t pos = code.size() - 1;
    for (;;) {
        uval >>= 7;
        uint8_t b = static_cast<uint8_t>(uval & 0x7f);
        bool has_more = !!(uval - b);
        code.push_back(b + 0x80);
        if (!has_more) {
            std::reverse(code.begin() + pos, code.end());
            return;
        }
    }
}

template <typename ContextT>
void builder<ContextT>::function_builder::materialize()
{
    dr_.address = builder_.vm_.get_ip(); // needs here to enable recursive calls

    // build blocks
    std::vector<block> blocks;
    blocks.emplace_back();

    for (instruction_entry& e : dr_.instructions) {
        switch (e.operation) {
        case op_t::noop: continue;
        case op_t::pushfp:
        case op_t::popfp:
        case op_t::pop:
        case op_t::ret:
            blocks.back().append(e.operation);
            break;
        case op_t::push:
        case op_t::pushc:
        case op_t::pushr:
        case op_t::pushi:
        case op_t::fppush:
        case op_t::fnpush:
        case op_t::fppushi:
        case op_t::fnpushi:
        case op_t::set:
        case op_t::setr:
        case op_t::fpset:
        case op_t::fnset:
        case op_t::truncatefpp:
        case op_t::truncatefpn:
            blocks.back().append(e.operation, get<size_t>(e.operand));
            break;
        case op_t::popn:
            {
                size_t num = get<size_t>(e.operand);
                if (num == 1) {
                    blocks.back().append(op_t::pop);
                } else if (num) {
                    blocks.back().append(e.operation, num);
                }
                break;
            }
        case op_t::ecall:
            {
                size_t fnindex = get<size_t>(e.operand);
                if (fnindex < 128) blocks.back().append(static_cast<op_t>(128 + fnindex));
                else blocks.back().append(e.operation, fnindex);
                break;
            }
        case op_t::collapse:
        {
            size_t num = get<size_t>(e.operand);
            if (!num) continue;
            blocks.back().append(e.operation, num);
            break;
        }
        case op_t::call:
        {
            function_identity const& fnident = get<function_identity_store_t>(e.operand);
            function_descriptor& fd = builder_.resolve_function(fnident);
            if (fd.address) {
                blocks.back().append(e.operation, *fd.address);
            } else {
                fd.index = builder_.allocate_constant_index();
                blocks.back().append(op_t::pushc, *fd.index);
                blocks.back().append(op_t::callp);
            }
            break;
        }

        default:
            THROW_NOT_IMPLEMENTED_ERROR("function_builder::materialize");
        }
    }

    if (blocks.size() == 1) {
        builder_.vm_.append(blocks.front().code);
        dr_.free(builder_);
        return;
    }
    THROW_NOT_IMPLEMENTED_ERROR("function_builder::materialize");
    
}

}
