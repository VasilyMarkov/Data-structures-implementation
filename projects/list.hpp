/*
 * Copyright (c) 2026 VasilyMarkov
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#pragma once 
#include <iterator>
#include <utility>
#include <exception>
#include <list>
#include "iterator.hpp"
#include "concepts.hpp"

namespace exp {

template<typename T>
class list {
public:

    class Node;
    struct BaseNode;

	// ============= Iterator ============= //
	
    template<bool isConst>
    class list_iterator: public iterator_facade<list_iterator<isConst>, std::conditional_t<isConst, const T, T>, std::bidirectional_iterator_tag> {
        using BaseType = iterator_facade<list_iterator<isConst>, std::conditional_t<isConst, const T, T>, std::bidirectional_iterator_tag>;
        friend list;
    public:
        using value_type = std::conditional_t<isConst, const typename BaseType::value_type, typename BaseType::value_type>; 
        using reference = std::conditional_t<isConst, const typename BaseType::reference, typename BaseType::reference>;
        using pointer = std::conditional_t<isConst, const typename BaseType::pointer, typename BaseType::pointer>;
        using iterator_category = typename BaseType::iterator_category;
        using difference_type = typename BaseType::difference_type;
    
        using base_node_pointer = std::conditional_t<isConst, const BaseNode*, BaseNode*>;
        using node_pointer = std::conditional_t<isConst, const Node*, Node*>;

        explicit list_iterator(base_node_pointer node) noexcept: base_node_(node) {}
        
        reference dereference() noexcept { return static_cast<node_pointer>(base_node_)->value_; }
        void increment() noexcept { base_node_ = base_node_->next_; }
        void decrement() noexcept { base_node_ = base_node_->prev_; }

        bool equal(const list_iterator& rhs) const noexcept { return base_node_ == rhs.base_node_; }
    private:
        base_node_pointer base_node_ = nullptr;
    };
	
	// ==================================== //

	using iterator = list_iterator<false>;
	using const_iterator = list_iterator<true>;

    struct BaseNode {
        BaseNode* next_ = nullptr;
        BaseNode* prev_ = nullptr;
    };

    class Node: public BaseNode {
        friend list;
        friend iterator;
    public:
        template<typename U> 
        Node(U&& val):value_(std::forward<U>(val)) {}

        BaseNode* asBase() { return static_cast<BaseNode*>(this); }

        friend std::ostream& operator<<(std::ostream& os, const Node& node) {
            return os << node.value_;
        }
    private:
        T value_;
    };

	list() {
		begin_.next_ = &begin_;
		begin_.prev_ = &begin_;
	}

    list(size_t n): list() {
        static_assert(DefaultConstructible<T>, "T isn't default constructible");
        size_t idx{};
        try
        {
            while(idx < n) {
                push_back({}); //TODO rewrite to emplace
                ++idx;
            }
        }
        catch(const std::exception&)
        {
            std::cout << "e" << std::endl;
            while(idx > 0) {
                pop_back();
                --idx;
            }
        }
    }

	//TODO write list unwind for strong exception garantee
	list(std::initializer_list<T> init_list): list() {
        size_t idx{};
        try
        {
            for(const auto& el : init_list) {
			    push_back(el);
                ++idx;
		    }
        }
        catch(const std::exception&)
        {
            std::cout << "e" << std::endl;
            while(idx > 0) {
                pop_back();
                --idx;
            }
        }
	}

    template<std::input_iterator It>
    list(It begin, It end): list() { //TODO write list unwind for strong exception garantee
        while(begin != end) {
            push_back(*begin);
            ++begin;
        }
    }

    list(const list& other): list(other.begin(), other.end()) {}

    list(list&& other) noexcept: list(), begin_(other.begin_) {}

	~list() {
        auto it = begin();
        auto end_it = end();
        while(it != end_it) {
            auto* curr_node =  static_cast<Node*>(it.base_node_);
            ++it;
            delete curr_node;
        }
	}
	
    iterator begin() noexcept { return iterator(begin_.next_); }
    const_iterator begin() const noexcept { return cbegin(); }
    const_iterator cbegin() const noexcept { return const_iterator(begin_.next_); }

    iterator end() noexcept { return iterator(&begin_); }
    const_iterator end() const noexcept { return cend(); }
    const_iterator cend() const noexcept { return const_iterator(&begin_); }

    iterator insert(iterator pos, const T& val) { //insert before
        
        Node* new_node = new Node(val);
        BaseNode* base = new_node->asBase();

        BaseNode* curr = pos.base_node_;
        BaseNode* prev = curr->prev_;

        base->next_ = curr;
        base->prev_ = prev;

        curr->prev_ = base;
        if (prev) {
            prev->next_ = base;
        }
        ++size_;

        return iterator(base);
    }
	
	size_t size() const noexcept { return size_; }

	void push_back(const T& val) {
		insert(end(), val);
	}
	void push_front(const T& val) {
		insert(begin(), val);
	}

    void erase(iterator pos) noexcept {
        BaseNode* curr = pos.base_node_;
        BaseNode* prev = curr->prev_;
        BaseNode* next = curr->next_;

        if(prev) {
            prev->next_ = next;
        }
        if(next) {
            next->prev_ = prev;
        }
        --size_;

        delete static_cast<Node*>(pos.base_node_);
    }

    void pop_back() noexcept {
        erase(std::prev(end()));
    }
    void pop_front() noexcept {
        erase(begin());
    }

    friend std::ostream& operator<<(std::ostream& os, const list& list) {
        for(const auto& el : list) {
            os << el << ' ';
        }
        return os;
    }

    friend bool operator==(const list& rhs, const list& lhs) noexcept {
        return std::equal(rhs.begin(), rhs.end(), lhs.begin());
    }
    friend bool operator!=(const list& rhs, const list& lhs) noexcept {
        return !(rhs == lhs);
    }

private:
	BaseNode begin_;
	size_t size_{};
};

template<typename T>
list(std::initializer_list<T>) -> list<T>;

template<std::input_iterator It>
list(It, It) -> list<typename std::iterator_traits<It>::value_type>;

} // namespace exp