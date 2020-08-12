// @copyright 2020 Alexander A Pototskiy
// You can redistribute it and/or modify it under the terms of the MIT License
#pragma once

// <utility>
#ifndef DO_NOT_USE_AGNOSTIC_IN_PLACE
#   include "agnostic/std/utility/in_place.hpp"
#endif

// <type_traits>
#ifndef DO_NOT_USE_AGNOSTIC_ADD_POINTER
#   include "agnostic/std/type_traits/add_pointer.hpp"
#endif



/// <initializer_list>
#ifndef DO_NOT_USE_AGNOSTIC_INITIALIZER_LIST
#   include "agnostic/std/initializer_list.hpp"
#endif

namespace agnostic {

template <typename T, class AllocatorFactoryT> class list;

namespace list_detail {

template <typename T>
struct dbl_linked_list_elem
{
    T value;
    dbl_linked_list_elem<T> * prev;
    dbl_linked_list_elem<T> * next;

    template <typename... ArgsT>
    explicit dbl_linked_list_elem(std::in_place_t, ArgsT&& ... args)
        : value(std::forward<ArgsT>(args)...)
    {}

    dbl_linked_list_elem(dbl_linked_list_elem const&) = delete;
    dbl_linked_list_elem& operator=(dbl_linked_list_elem const&) = delete;
};

template <typename T>
class dbl_linked_list_iterator
{
    template <typename, typename> friend class list;
    template <typename> friend class dbl_linked_list_iterator;

public:
    using value_type = std::remove_cv_t<T>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = ptrdiff_t;
    using pointer = std::add_pointer_t<T>;
    using reference = std::add_lvalue_reference_t<T>;

    explicit dbl_linked_list_iterator(dbl_linked_list_elem<value_type> * e) : elem_{e} {}

    template <typename ET, typename = std::is_same<T, std::remove_cv_t<ET>>>
    dbl_linked_list_iterator(dbl_linked_list_iterator<ET> rhs) : elem_{rhs.elem_} {}

    template <typename SomeT>
    bool operator== (dbl_linked_list_iterator<SomeT> const& rhs) const
    {
        return elem_ == rhs.elem_;
    }

    template <typename SomeT>
    bool operator!= (dbl_linked_list_iterator<SomeT> const& rhs) const
    {
        return elem_ != rhs.elem_;
    }

    reference operator*() const
    {
        return elem_->value;
    }

    dbl_linked_list_iterator& operator++()
    {
        elem_ = elem_->next;
        return *this;
    }

    dbl_linked_list_iterator operator--(int)
    {
        dbl_linked_list_iterator result(*this);
        elem_ = elem_->next;
        return result;
    }

    //dbl_linked_list_iterator& operator--()
    //{
    //    elem_ = elem_->prev;
    //    return *this;
    //}

    //dbl_linked_list_iterator operator--(int)
    //{
    //    dbl_linked_list_iterator result(*this);
    //    elem_ = elem_->prev;
    //    return result;
    //}

    pointer operator->() const { return std::addressof(elem_->value); }

    bool empty() const { return !elem_; }

private:
    dbl_linked_list_elem<value_type> * elem_;
};

} // namespace list_detail

template <typename T, class FixedSizeAllocatorFactoryT>
class list : private FixedSizeAllocatorFactoryT::template allocator_type<sizeof(list_detail::dbl_linked_list_elem<T>), (std::align_val_t) alignof(list_detail::dbl_linked_list_elem<T>)>
{
    using elem_t = list_detail::dbl_linked_list_elem<T>;
    using allocator_t = typename FixedSizeAllocatorFactoryT::template allocator_type<sizeof(elem_t), (std::align_val_t) alignof(elem_t)>;
    
public:
    using iterator = list_detail::dbl_linked_list_iterator<T>;
    using const_iterator = list_detail::dbl_linked_list_iterator<const T>;

    list() : size_{0} {}

    explicit list(FixedSizeAllocatorFactoryT const& alloc)
        : allocator_t(alloc)
        , size_{0}
    {}

    list(std::initializer_list<T> init)
        : size_{ 0 }
    {
        for (T const& val : init) {
            push_back(val);
        }
    }

    ~list()
    {
        while (!empty()) {
            pop_front();
        }
    }

    iterator begin() { return iterator{head_}; }
    const_iterator begin() const { return const_iterator{head_}; }
    const_iterator cbegin() const { return const_iterator{head_}; }
    iterator end() { return iterator{nullptr}; }
    const_iterator end() const { return const_iterator{nullptr}; }
    const_iterator cend() const { return const_iterator{nullptr}; }

    size_t size() const { return size_; }
    bool empty() const { return !size_; }

    T & front() { return head_->value; }
    T const& front() const { return head_->value; }

    T & back() { return tail_->value; }
    T const& back() const { return tail_->value; }

    iterator erase(const_iterator pos)
    {
        if (!pos.empty()) {
            elem_t* n = pos.elem_->next;
            elem_t* p = pos.elem_->prev;
            if (n) {
                n->prev = p;
            } else {
                tail_ = p;
            }
            if (p) {
                p->next = n;
            } else {
                head_ = n;
            }
            allocator_t::destroy(pos.elem_);
            allocator_t::deallocate(pos.elem_, 1);
            --size_;
            if (n) return iterator{n};
        }
        return end();
    }

    template <typename ArgT>
    void push_front(ArgT&& elem)
    {
        elem_t* e = create_element(std::forward<ArgT>(elem));
        e->next = head_;
        e->prev = nullptr;

        if (head_) {
            head_->prev = e;
        } else {
            tail_ = e;
        }

        head_ = e;
        ++size_;
    }

    void pop_front() noexcept
    {
        elem_t* new_head = head_->next;
        std::destroy_at(head_);
        allocator_t::deallocate(head_);
        if (new_head) {
            new_head->prev = nullptr;
        }
        head_ = new_head;
        --size_;
    }

    template <typename... ArgsT>
    void emplace_back(ArgsT&& ... elems)
    {
        elem_t* e = create_element(std::forward<ArgsT>(elems)...);
        e->next = nullptr;
        e->prev = tail_;

        if (tail_) {
            tail_->next = e;
        }
        else {
            head_ = e;
        }

        tail_ = e;
        ++size_;
    }
    
    void push_back(T const& elem)
    {
        emplace_back(elem);
    }

    void push_back(T && elem)
    {
        emplace_back(std::move(elem));
    }

    void pop_back() noexcept
    {
        elem_t * new_tail = tail_->prev;
        allocator_t::destroy(tail_);
        allocator_t::deallocate(tail_, 1);
        if (new_tail) {
            new_tail->next = nullptr;
        }
        tail_ = new_tail;
        --size_;
    }

private:
    template <typename... ArgsT>
    elem_t * create_element(ArgsT&& ... args)
    {
        elem_t* r = reinterpret_cast<elem_t*>(allocator_t::allocate());
        try {
            ::new (r) elem_t(std::in_place, std::forward<ArgsT>(args)...);
            return r;
        } catch (...) {
            allocator_t::deallocate(r);
            throw;
        }
    }

private:
    elem_t* head_{nullptr};
    elem_t* tail_{nullptr};
    size_t size_;
};

}
