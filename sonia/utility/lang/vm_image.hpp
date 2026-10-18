//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one

#pragma once

#include <array>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "sonia/string.hpp"
#include "sonia/span.hpp"
#include "sonia/exceptions.hpp"
#include "sonia/utility/iterators/safe_iterator.hpp"
#include "sonia/utility/iterators/empty_check_iterator.hpp"
#include "sonia/utility/serialization/serialization.hpp"
#include "sonia/utility/serialization/integral.hpp"
#include "sonia/utility/serialization/span.hpp"
#include "sonia/utility/serialization/string.hpp"
#include "sonia/utility/serialization/vector.hpp"
#include "sonia/utility/serialization/map.hpp"
#include "sonia/utility/serialization/blob_result.hpp"

#include "vm2.hpp"

namespace sonia::vm {

// A precompiled program image: the complete state of a virtual_stack_machine right after the
// compilation, before the program has been run - code, constants, the extern function table, the
// address descriptions and the entry point. Loading an image skips the compilation entirely, the
// program is just run from its entry point. The front end (language) supplies image_traits.
//
// The image is bound to the runtime it was built for: any version mismatch, or an extern function
// the image refers to and the runtime doesn't provide, fails the load with an exception.
//
// Constants must be smart_blobs (see the blob_result coder for what is storable). Extern functions
// are bound by index for the first `indexed_efn_count` ones (the front end's fixed builtins), by
// their signature (the efn description) for the rest, so those must have unique signatures.
//
// Layout (sonia::serialization, compressed_t: `vuint` is an unsigned LEB128, `str` is vuint size + bytes):
//   magic (4 bytes); vuint format_version; vuint sonia::vm::bytecode_version; str source name
//   vuint efn count;         per efn:         u8 kind (0 absent, 1 by index, 2 by signature) [str signature]
//   vuint constant count;    per constant:    blob_result
//   vuint code size;         code bytes
//   vuint entry address
//   vuint description count; per description: vuint address, str text

struct image_traits
{
    std::array<uint8_t, 4> magic;
    // the front end's image version: bump on changes of the layout below or of the contract between
    // the front end's compiled code and its runtime
    uint32_t format_version;
    string_view language;    // for messages, e.g. "annium"
    size_t indexed_efn_count;
};

namespace image_detail {

using tag_t = serialization::compressed_t;

enum class efn_kind : uint8_t { absent = 0, by_index = 1, by_signature = 2 };

// The input iterator checks its end and knows how much is left, so the decoders refuse sizes that
// can't fit (see check_decoded_count); a truncated or malformed image fails in a decoder (eof or
// malformed data) and is reported as corrupted.
class reader
{
    using iterator_t = empty_check_iterator<safe_iterator<uint8_t const*>>;

public:
    reader(span<const uint8_t> data, image_traits const& traits, string_view name)
        : it_{ in_place, data.data(), data.data() + data.size() }, traits_{ traits }, name_{ name }
    {}

    [[noreturn]] void corrupted() const
    {
        throw exception("%1% image '%2%' is corrupted (truncated or malformed)"_fmt % traits_.language % name_);
    }

    template <typename T>
    T get()
    {
        try {
            T v;
            it_ = sonia::decode<tag_t>(std::move(it_), v);
            return v;
        } catch (exception const&) {
            corrupted();
        }
    }

    // the count of the T elements that follow, to read them one by one
    template <typename T>
    size_t count()
    {
        uint64_t n = get<uint64_t>();
        try {
            check_decoded_count<tag_t, T>(it_, n);
        } catch (eof_exception const&) {
            corrupted();
        }
        return static_cast<size_t>(n);
    }

    bool empty() const { return it_.empty(); }

private:
    iterator_t it_;
    image_traits const& traits_;
    string_view name_;
};

}

template <typename ContextT>
requires(std::is_same_v<typename ContextT::variable_type, smart_blob>)
std::vector<uint8_t> save_image(virtual_stack_machine<ContextT> const& vm, image_traits const& traits, size_t entry_address, string_view source_name,
    bool with_descriptions = true) // the descriptions are diagnostics only, without them the table is empty
{
    using namespace image_detail;

    std::vector<uint8_t> result;
    auto enc = make_encoder<tag_t>(std::back_inserter(result));
    enc & std::span<const uint8_t, 4>{ traits.magic } & traits.format_version & bytecode_version & source_name;

    auto const& efns = vm.efns();
    std::unordered_set<string_view> signatures;
    enc & efns.size();
    for (size_t i = 0; i < efns.size(); ++i) {
        auto const& [pfn, descr] = efns[i];
        if (!pfn) {
            enc & efn_kind::absent;
        } else if (i < traits.indexed_efn_count) {
            enc & efn_kind::by_index;
        } else {
            string_view sig = descr;
            if (sig.empty()) {
                throw exception("extern function #%1% has no signature, it can't be stored in %2% image"_fmt % i % traits.language);
            }
            if (!signatures.insert(sig).second) {
                throw exception("extern function signature '%1%' is not unique, it can't be stored in %2% image"_fmt % sig % traits.language);
            }
            enc & efn_kind::by_signature & sig;
        }
    }

    auto const& consts = vm.consts();
    enc & consts.size();
    for (smart_blob const& c : consts) {
        enc & *c;
    }

    enc & vm.code() & entry_address;
    if (with_descriptions) {
        enc & vm.address_descriptions();
    } else {
        enc & std::unordered_map<size_t, std::string>{};
    }
    return result;
}

// Replaces the code, constants, extern function table and address descriptions of `vm` with the
// image's ones, binding the image's extern functions to the ones `vm` already has. Returns the
// entry address. `image_name` is used in error messages only.
template <typename ContextT>
requires(std::is_same_v<typename ContextT::variable_type, smart_blob>)
size_t load_image(virtual_stack_machine<ContextT>& vm, image_traits const& traits, span<const uint8_t> image, string_view image_name)
{
    using namespace image_detail;
    using efns_t = std::remove_cvref_t<decltype(vm.efns())>;

    if (image.size() < traits.magic.size() || !std::equal(traits.magic.begin(), traits.magic.end(), image.begin())) {
        throw exception("'%1%' is not %2% image"_fmt % image_name % traits.language);
    }
    reader r{ image.subspan(traits.magic.size()), traits, image_name };

    if (auto v = r.get<uint32_t>(); v != traits.format_version) {
        throw exception("%1% image '%2%' has format version %3%, the runtime expects %4%: rebuild the image"_fmt
            % traits.language % image_name % v % traits.format_version);
    }
    if (auto v = r.get<uint32_t>(); v != bytecode_version) {
        throw exception("%1% image '%2%' has bytecode version %3%, the runtime expects %4%: rebuild the image"_fmt
            % traits.language % image_name % v % bytecode_version);
    }
    r.get<std::string>(); // source name, informational

    // extern functions: by index or by signature
    efns_t const& runtime_efns = vm.efns();
    constexpr size_t ambiguous = (size_t)-1;
    std::unordered_map<string_view, size_t> by_signature;
    for (size_t i = traits.indexed_efn_count; i < runtime_efns.size(); ++i) {
        auto const& [pfn, descr] = runtime_efns[i];
        if (!pfn) continue;
        auto [it, inserted] = by_signature.emplace(string_view{ descr }, i);
        if (!inserted) it->second = ambiguous;
    }

    efns_t efns(r.count<efn_kind>());
    for (size_t i = 0; i < efns.size(); ++i) {
        switch (r.get<efn_kind>()) {
        case efn_kind::absent:
            break;
        case efn_kind::by_index:
            if (i >= traits.indexed_efn_count || i >= runtime_efns.size() || !runtime_efns[i].first) {
                throw exception("%1% image '%2%': builtin extern function #%3% is not provided by this runtime"_fmt % traits.language % image_name % i);
            }
            efns[i] = runtime_efns[i];
            break;
        case efn_kind::by_signature:
            {
                std::string sig = r.get<std::string>();
                auto it = by_signature.find(sig);
                if (it == by_signature.end()) {
                    throw exception("%1% image '%2%': extern function '%3%' is not provided by this runtime"_fmt % traits.language % image_name % sig);
                }
                if (it->second == ambiguous) {
                    throw exception("%1% image '%2%': extern function '%3%' is ambiguous in this runtime"_fmt % traits.language % image_name % sig);
                }
                efns[i] = runtime_efns[it->second];
            }
            break;
        default:
            r.corrupted();
        }
    }

    size_t const_count = r.count<blob_result>();
    std::vector<smart_blob> consts;
    consts.reserve(const_count);
    for (size_t i = 0; i < const_count; ++i) {
        consts.emplace_back(r.get<blob_result>()); // takes over the decoded blob
    }

    auto code = r.get<std::vector<uint8_t>>();
    uint64_t entry_address = r.get<uint64_t>();
    if (entry_address >= code.size()) r.corrupted();

    auto descriptions = r.get<std::unordered_map<size_t, std::string>>();

    if (!r.empty()) r.corrupted();

    // the whole image is parsed and bound, now replace the machine state
    vm.set_code(std::move(code));
    vm.set_consts(std::move(consts));
    vm.set_efns(std::move(efns));
    vm.set_address_descriptions(std::move(descriptions));
    return static_cast<size_t>(entry_address);
}

}
