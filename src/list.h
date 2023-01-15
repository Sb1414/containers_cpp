#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#include <iostream>
using namespace std;

template <typename T>
class s21_list {
    using pointer = T*;
    using const_reference = const T&;
    using value_type = T;
    using reference = T&; 
    using size_type = unsigned int;
    // using Node = ListNode;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:
    struct ListNode {
        ListNode* prev; // адрес предыдущего элемента в списке
        ListNode* next; // адрес следующего элемента в списке
        value_type data; // данные
        ListNode() : data(), prev(nullptr), next(nullptr) {}
    };
    // T* arr;
    // int size_;

public:
    using Node = ListNode;

private:
    size_type size_;  // количество элементов в списке
    Node* begin_;  // указатель на первый элемент списка
    Node* end_;  // указатель на последний элемент списка

public:
    class ConstIterator;
    // s21_list(int size = 1) : size_(size) {
    //     arr = new T[size];
    // }

    // T& operator[] (const int& size) {
    //     if (size > 0 && size < size_)
    //         return arr[size];
    //     return arr[0];
    // }

    // ConstIterator begin() { return ConstIterator(begin_); }
    // ConstIterator end() { return ConstIterator(begin_ + size_); }

    class ConstIterator {
    // protected:
        // Node* node_;
        
    public:
        Node* node_;

        ConstIterator() = default;
        // ConstIterator() { this.node = nullptr; } // либо то что выше, либо это
        ConstIterator(Node* data) { this->node_ = data; }
        ConstIterator(const ConstIterator& other) : node_(other.node_) {}
        ~ConstIterator() {}

        ConstIterator& operator++() {
            this->node_ = this->node_->next;
            return *this;
        }

        ConstIterator operator++ (int) { 
            ConstIterator tmp(this->node_);
            this->node_ = this->node_->next;
            return tmp;
        }

        ConstIterator& operator--() {
            this->node_ = this->node_->prev;
            return *this;
        }

        ConstIterator operator-- (int) { 
            ConstIterator tmp(this->node_);
            this->node_ = this->node_->prev;
            return tmp;
        }

        ConstIterator operator+ (size_type n) { // пересмотреть потом
            ConstIterator tmp(this->node_->data + n);
            return tmp; 
        }
        ConstIterator operator- (size_type n) {  // пересмотреть потом
            ConstIterator tmp(this->node_->data - n);
            return tmp;  
        }

        bool operator!= (const ConstIterator& other) { return !(this->node_ == other.node_); }
        bool operator== (const ConstIterator& other) { return this->node_ == other.node_; }

        const_reference operator* () {
            if (this->node_ == nullptr) 
                throw std::invalid_argument("No data");
            return this->node_->data; 
        }
    };

    class Iterator : public ConstIterator {
    public:
        Iterator() : ConstIterator() {}
        Iterator(Node* data) : ConstIterator(data) {}
        Iterator(const Iterator& other) : : ConstIterator(other) {}
        Iterator(const ConstIterator& other) : : ConstIterator(other) {}
        ~Iterator() {}

        Iterator& operator++() {
            this->node_ = this->node_->next;
            return *this;
        }

        Iterator operator++ (int) { 
            Iterator tmp(this->node_);
            this->node_ = this->node_->next;
            return tmp;
        }

        Iterator& operator--() {
            this->node_ = this->node_->prev;
            return *this;
        }

        Iterator operator-- (int) { 
            Iterator tmp(this->node_);
            this->node_ = this->node_->prev;
            return tmp;
        }

        Iterator operator+ (const size_type n) { // пересмотреть потом
            Iterator tmp(this->node_->data + n);
            return tmp; 
        }

        Iterator operator- (const size_type n) {  // пересмотреть потом
            Iterator tmp(this->node_->data - n);
            return tmp;  
        }

        Iterator& operator= (const Iterator other) { // не видит node
            this->node_ = other->node_;
            return tmp;  
        }

        reference operator* () { 
            if (this->node_ == nullptr) 
                throw std::invalid_argument("No data");
            return this->node_->data; 
        }

    };

    public:
        // using iterator = Iterator;
        // using const_iterator = ConstIterator;

        list() {
            begin_ = end_ = new Node;
            size_ = 0;
        }

        list(const list& l) {
            begin_ = end_ = new Node;
            size_ = 0;
            for (auto i : l) {
                this->push_back(i);
            }
        }

        list(list&& l) {
            if (this == &l) {
                throw std::invalid_argument("Error, the lists are the same");
            }
            end_ = l.end_;
            begin_ = l.begin_;
            size_ = l.size_;
            l.begin_ = nullptr;
            l.end_ = nullptr;
            l.size_ = 0;
        }

        iterator insert(iterator pos, const_reference value) {
            ++size_;
            Node* node = new Node;
            node->data = value;
            node->next = pos._node;

            if (pos._node == begin_) {
                begin_ = node;
            } else {
                pos->prev->next = node;
                node->prev = pos->prev;
            }
            pos->prev = node;
            return iterator(node);
        }
  
        void push_back(const_reference value) {
            this->insert(iterator(end_), value);
        }
        void push_front(const_reference value) {
            this->insert(iterator(begin_), value);
        }

        const_iterator begin() const { return const_iterator(begin_); }
        const_iterator end() const { return const_iterator(end_); }
        iterator begin() { return iterator(begin_); }
        iterator end() { return iterator(end_); }
};

#endif  // SRC_LIST_H_