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
	class list_iterator;
	using iterator = list_iterator;

	class Node {
		friend list;
		friend iterator;
	public:
		template<typename U>
		Node(U&& val):val_(std::forward<U>(val)) {}

		~Node() {
			if(next_) {
				delete next_;
			}
		}
	private:
		Node* next_ = nullptr;
		Node* prev_ = nullptr;
		T val_;
	};

	// ============= Iterator ============= //
	
	class list_iterator: public iterator_facade<
									list_iterator, 
									T, 
									std::bidirectional_iterator_tag
								> {
		using BaseType =  iterator_facade<
							list_iterator, 
							T, 
							std::bidirectional_iterator_tag
						  >;
		using reference = typename BaseType::reference;
	public:
		list_iterator(Node* node) noexcept :node_(node) {}

		reference dereference() noexcept { return node_->val_; }

		void increment() noexcept { node_ = node_->next_; }
		void decrement() noexcept { node_ = node_->prev_; }
		void advance(size_t n) noexcept {
			increment();
		}
		bool equal(const list_iterator& other) const {
			return node_ == other.node_;
		}
	private:
		Node* node_ = nullptr;
	};
	
	// ==================================== //
	
	list() = default;
	
	list(size_t n) { //TODO write list unwind for strong exception garantee
		size_t idx{};
		try {
			while(idx < n) {
				push_back(T{}); //TODO check T by concept default_initializable
				idx++;
			}
			size_ = n;
		}
		catch(const std::exception& e) {
			// while(idx != 0) {

			// 	--idx;
			// }	
		}
	}

	//TODO write list unwind for strong exception garantee
	list(std::initializer_list<T> list) {
		for(const auto& el : list) {
			push_back(el);
		}
		size_ = list.size();
	}

	~list() {
		destroyNode(head_);
	}

	void push_back(const T& val) {
		if(!head_) {
			head_ = constructNode(val);
			tail_ = head_;
		}
		else {
			auto* node = constructNode(val);
			node->prev_ = tail_;
			tail_->next_ = node;
			tail_ = tail_->next_;
		}
		++size_;
	}

	void pop_back() {
		// destroyNode(tail_);
	}

	iterator begin() noexcept { return iterator(head_); }
	iterator end() noexcept { return iterator(tail_->next_); }

	size_t size() const noexcept { return size_; }

private:
	template<typename U>
	Node* constructNode(U&& val) {
		return new Node(std::forward<U>(val));
	}

	void destroyNode(Node* node) {
		if(node) {
			delete node;
		}
	}

	Node* head_ = nullptr;
	Node* tail_ = nullptr;
	size_t size_{};
};
