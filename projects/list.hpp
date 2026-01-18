/*
 * Copyright (c) 2026 VasilyMarkov
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#pragma once 
#include <iterator>
#include <utility>
#include <exception>
#include <type_traits>
#include <list>
#include "iterator.hpp"

template<typename T>
class list {
public:
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

        explicit list_iterator(BaseNode* node) noexcept: base_node_(node) {}
        
        reference dereference() noexcept { return static_cast<Node*>(base_node_)->value_; }
        void increment() noexcept { base_node_ = base_node_->next_; }
        void decrement() noexcept { base_node_ = base_node_->prev_; }

        bool equal(const list_iterator& rhs) const noexcept { return base_node_ == rhs.base_node_; }
    private:
        BaseNode* base_node_ = nullptr;
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

    list(size_t n) {
        size_t idx{};
        //TODO write list unwind for strong exception garantee
        while(idx < n) {
            //TODO check T is default constructible
            push_back(T{}); //TODO rewrite to emplace
            ++idx;
        }
    }

	//TODO write list unwind for strong exception garantee
	// list(std::initializer_list<T> list) {
		// for(const auto& el : list) {
		// 	push_back(el);
		// }
		// size_ = list.size();
	// }

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
    iterator end() noexcept { return iterator(&begin_); }

    iterator insert(iterator pos, const T& val) {
        
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
private:
	BaseNode begin_;
	size_t size_{};
};
