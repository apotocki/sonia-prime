//  Sonia.one framework (c) by Alexander A Pototskiy
//  Sonia.one is licensed under the terms of the Open Source GPL 3.0 license.
//  For a license to use the Sonia.one software under conditions other than those described here, please contact me at admin@sonia.one
#include "sonia/config.hpp"

#include "applied/sonia_test.hpp"

#include <type_traits>
#include <utility>
#include <list>

#define DO_NOT_USE_AGNOSTIC_TYPE_IDENTITY
#define DO_NOT_USE_AGNOSTIC_ADDRESSOF
#define DO_NOT_USE_AGNOSTIC_CONDITIONAL
#define DO_NOT_USE_AGNOSTIC_IS_SAME

#include "agnostic/std/type_traits/type_identity.hpp"
#include "agnostic/intrusive/avl.hpp"


namespace {

template <typename KT, typename ValueT>
class element // : public agnostic::intrusive::avl_tree_base_hook<>
{
public:
    template <typename SKT, typename SVT>
    element(SKT&& k, SVT&& v) : key{ std::forward<SKT>(k) }, value{ std::forward<SVT>(v) } {}

    bool operator< (element const& e) const { return key < e.key; }
    bool operator== (element const& e) const { return key == e.key; }
    bool operator!= (element const& e) const { return key != e.key; }

    KT key;
    ValueT value;

    agnostic::intrusive::avl_tree_member_hook<> hook_;
};

template <typename KT, typename ValueT>
inline bool operator< (element<KT, ValueT> const& l, int r) { return l.key < r; }

template <typename KT, typename ValueT>
inline bool operator< (int l, element<KT, ValueT> const& r) { return l < r.key; }

}

#include <random>
#include <algorithm>

#if 0
BOOST_AUTO_TEST_CASE(primitive_avl_test)
{
    using elem_t = element<int, int>;
    //using avl_tree_t = agnostic::intrusive::avl_tree<elem_t>;
    using avl_tree_t = agnostic::intrusive::avl_tree<
        elem_t,
        agnostic::intrusive::member_hook<elem_t, agnostic::intrusive::avl_tree_member_hook<>, &elem_t::hook_>
    >;

    int keys[1024];

    std::random_device rd;

    for (int k = 0; k < 1024 * 16; ++k) {
        auto rdval = rd();
        std::mt19937 g(rdval);
        //std::mt19937 g(0x2d70f40e);
        std::cout << "attempt #" << k << ", rdval = " << rdval;

        avl_tree_t tree;
        std::list<elem_t> elems;

        size_t keys_sz = sizeof(keys) / sizeof(int);
        for (size_t i = 0; i < keys_sz; ++i) {
            keys[i] = (int)i;
        }

        

        std::shuffle(keys, keys + keys_sz, g);

        for (size_t i = 0; i < keys_sz; ++i) {
            elems.emplace_back(keys[i], keys[i] + 1);
            tree.insert(elems.back());
            tree.check();
        }

        auto h = tree.check();
        std::cout << ", height= " << h << "\n";

        for (size_t i = 0; i < keys_sz; ++i) {
            auto it = tree.find((int)i);
            assert(!it.empty());
            assert(it->key == (int)i);
            assert(it->value == (int)i + 1);
        }

        int i = 0;
        auto it = tree.begin();
        for (; i < keys_sz; ++i, ++it) {
            assert(i == it->key);
        }
        assert(it.empty());

        for (size_t i = 0; i < keys_sz; ++i) {
            auto it = tree.find(keys[i]);
            it = tree.remove_node(*it);
            assert(it.empty() || it->key > keys[i]);

            std::vector<int> tmpvals(&keys[i] + 1, keys + keys_sz);
            std::sort(tmpvals.begin(), tmpvals.end());
            auto b = tree.begin();
            for (int e : tmpvals) {
                assert (!b.empty());
                assert (e == b->key);
                ++b;
            }
            assert (b.empty());

    #if 0
            int si  =-1;
            for (auto const& e : tree) {
                assert(e.key > si);
                si = e.key;
            }
    #endif
            tree.check();
        }
    }
}
#endif

#include "agnostic/avl_set.hpp"

void avl_set_test()
{
    using avl_set_t = agnostic::avl_set<int>;

    int keys[1024];

    std::random_device rd;

    for (int k = 0; k < 16; ++k) {
        auto rdval = rd();
        std::mt19937 g(rdval);
        //std::mt19937 g(0x2d70f40e);
        std::cout << "attempt #" << k << ", rdval = " << rdval << "\n";

        avl_set_t set;

        size_t keys_sz = sizeof(keys) / sizeof(int);
        for (size_t i = 0; i < keys_sz; ++i) {
            keys[i] = (int)i;
        }

        std::shuffle(keys, keys + keys_sz, g);

        for (size_t i = 0; i < keys_sz; ++i) {
            auto [it, inserted] = set.insert(keys[i]);
            assert (inserted);
            assert(set.size() == i + 1);
            //elems.emplace_back(keys[i], keys[i] + 1);
            //tree.insert(elems.back());
            set.check_structure();
        }

        //std::cout << ", height= " << h << "\n";

        for (size_t i = 0; i < keys_sz; ++i) {
            auto it = set.find((int)i);
            assert(!it.empty());
            assert(*it == (int)i);
        }

        int i = 0;
        auto it = set.begin();
        for (; i < keys_sz; ++i, ++it) {
            assert(i == *it);
        }
        assert(it.empty());
#if 1
        for (size_t i = 0; i < keys_sz; ++i) {
            avl_set_t::iterator it = set.find(keys[i]);
            it = set.erase(it);
            assert(it.empty() || *it > keys[i]);
            assert(set.size() == keys_sz - i - 1);

            std::vector<int> tmpvals(&keys[i] + 1, keys + keys_sz);
            std::sort(tmpvals.begin(), tmpvals.end());
            auto b = set.begin();
            for (int v : tmpvals) {
                assert(!b.empty());
                assert(v == *b);
                b++;
            }
            assert(b.empty());
            auto e = set.end();
            for (auto vit = tmpvals.rbegin(), evit = tmpvals.rend(); vit != evit; ++vit) {
                --e;
                assert(*vit == *e);
            }
            assert(e == set.begin());
#if 0
            int si = -1;
            for (auto const& e : tree) {
                assert(e.key > si);
                si = e.key;
            }
#endif
            set.check_structure();
        }
#endif
   }
}

void agnostic_avl_test_registrar()
{
    register_test(BOOST_TEST_CASE(&avl_set_test));
}

#ifdef AUTO_TEST_REGISTRATION
AUTOTEST(agnostic_avl_test_registrar)
#endif
