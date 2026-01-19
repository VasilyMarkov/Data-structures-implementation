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

	template<bool isConst>
	class reverse_list_iterator: public iterator_facade<list_iterator<isConst>, std::conditional_t<isConst, const T, T>, std::bidirectional_iterator_tag> {
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

		explicit reverse_list_iterator(base_node_pointer node) noexcept: base_node_(node) {}
		
		reference dereference() noexcept { return static_cast<node_pointer>(base_node_)->value_; }
		void increment() noexcept { base_node_ = base_node_->prev_; }
		void decrement() noexcept { base_node_ = base_node_->next_; }

		bool equal(const reverse_list_iterator& rhs) const noexcept { return base_node_ == rhs.base_node_; }
	private:
		base_node_pointer base_node_ = nullptr;
	};
	
	// ==================================== //

	using iterator = list_iterator<false>;
	using const_iterator = list_iterator<true>;
	using reverse_iterator = reverse_list_iterator<false>;
	using const_reverse_iterator = reverse_list_iterator<true>;

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

		template<typename... Args> 
		Node(Args&&... args):value_(std::forward<Args>(args)...) {}

		BaseNode* asBase() { return static_cast<BaseNode*>(this); }

		friend std::ostream& operator<<(std::ostream& os, const Node& node) {
			return os << node.value_;
		}
	private:
		T value_;
	};

	iterator begin() noexcept { return iterator(begin_.next_); }
	const_iterator begin() const noexcept { return cbegin(); }
	const_iterator cbegin() const noexcept { return const_iterator(begin_.next_); }
	
	iterator end() noexcept { return iterator(&begin_); }
	const_iterator end() const noexcept { return cend(); }
	const_iterator cend() const noexcept { return const_iterator(&begin_); }
	
	reverse_iterator rbegin() noexcept { return iterator(begin_.next_); }
	const_reverse_iterator rbegin() const noexcept { return crbegin(); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(); }
	
	reverse_iterator rend() noexcept { return end(); }
	const_reverse_iterator rend() const noexcept { return crend(); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(); }

	list() {
		begin_.next_ = &begin_;
		begin_.prev_ = &begin_;
		// std::cout << "IS BASE OF: " << std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<decltype(begin())>::iterator_category> << std::endl;
		// std::cout << "IS A: " << std::is_same_v<typename std::iterator_traits<decltype(begin())>::iterator_category, std::bidirectional_iterator_tag> << std::endl;
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

	// template<std::input_iterator It> //TODO Fix candidate template ignored: constraints not satisfied
	template<typename It>
	list(It begin, It end): list() { //TODO write list unwind for strong exception garantee
		while(begin != end) {
			push_back(*begin);
			++begin;
		}
	}

	list(const list& other): list(other.begin(), other.end()) {}
	list(list&& other) noexcept: begin_(other.begin_) {}

	// list& operator=(const list& other) {
	//     if(*this == &other) return *this;

	//     auto begin = other.size() > size() ? other.begin() : begin(); 

	//     return *this;
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

	iterator insert(iterator pos, const T& val) { //insert before
		Node* new_node = new Node(val);

		return insertNodeImpl(pos, new_node);
	}

	template<typename... Args>
	iterator emplace(iterator pos, Args&&... args) {
		Node* new_node = new Node(std::forward<Args>(args)...);

		return insertNodeImpl(pos, new_node);
	}

	size_t size() const noexcept { return size_; }

	void push_back(const T& val) { insert(end(), val); }
	void push_back(T&& val) { insert(end(), std::move(val)); }

	void push_front(const T& val) { insert(begin(), val); }
	void push_front(T&& val) { insert(begin(), std::move(val)); }

	template<typename... Args>
	void emplace_back(Args&&... args) { emplace(end(), std::forward<Args>(args)...); }

	template<typename... Args>
	void emplace_front(Args&&... args) { emplace(begin(), std::forward<Args>(args)...); }

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
	void linkNodeTo(BaseNode* new_node, BaseNode* curr) noexcept {
		BaseNode* prev = curr->prev_;

		new_node->next_ = curr;
		new_node->prev_ = prev;

		curr->prev_ = new_node;
		if (prev) {
			prev->next_ = new_node;
		}
	}

	iterator insertNodeImpl(iterator pos, Node* node) noexcept {
		BaseNode* new_base_node = node->asBase();

		linkNodeTo(new_base_node, pos.base_node_);

		++size_;

		return iterator(new_base_node);
	}

	BaseNode begin_;
	size_t size_{};
};

template<typename T>
list(std::initializer_list<T>) -> list<T>;

template<std::input_iterator It>
list(It, It) -> list<typename std::iterator_traits<It>::value_type>;

} // namespace exp