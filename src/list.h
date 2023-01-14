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
    using const_reference = const T&;
    using size_type = unsigned int;

private:
    struct ListNode {
        ListNode* prev;
        ListNode* next;
        value_type data;
        ListNode() : data(), prev(nullptr), next(nullptr) {}
    };
    // T* arr;
    // int size_;

public:
    using Node = ListNode;

private:
    size_type _size;
    Node* _front;
    Node* _back;

public:
    class ConstIterator;
    s21_list(int size = 1) : size_(size) {
        arr = new T[size];
    }

    T& operator[] (const int& size) {
        if (size > 0 && size < size_)
            return arr[size];
        return arr[0];
    }

    ConstIterator begin() { return ConstIterator(_front); }
    ConstIterator end() { return ConstIterator(_front + _size); }

    // ConstIterator begin() { return arr; }
    // ConstIterator end() { return arr+size_; }

    // friend ostream& operator<< (ostream& s, const s21_list<T>& size);

    class ConstIterator {
    protected:
        Node* node_;
        
    public:
        ConstIterator() = default;
        // ConstIterator() { this.node = nullptr; } // либо то что выше, либо это
        ConstIterator(Node* data) { this->node_ = data; }
        ConstIterator(const ConstIterator& other) : node_(other.node_) {}
        ~ConstIterator() {}

        ConstIterator& operator++() {
            this->node_ = this->node_->next;
            return *this;
        }

        ConstIterator& operator++ () { 
            ConstIterator tmp(this->node_);
            this->node_ = this->node_->next;
            return tmp;
        }

        ConstIterator& operator--() {
            this->node_ = this->node_->prev;
            return *this;
        }

        ConstIterator& operator-- () { 
            ConstIterator tmp(this->node_);
            this->node_ = this->node_->prev;
            return tmp;
        }

        ConstIterator& operator+ (size_type n) { // пересмотреть потом
            ConstIterator tmp(this->node_->data + n);
            return tmp; 
        }
        ConstIterator& operator- (size_type n) {  // пересмотреть потом
            ConstIterator tmp(this->node_->data - n);
            return tmp;  
        }

        bool operator!= (const ConstIterator& other) { return !(this->node_ == other.node_); }
        bool operator== (const ConstIterator& other) { return this->node_ == other.node_; }

        ConstIterator& operator* () { 
            if (this->node_ == nullptr) 
                throw std::invalid_argument("No data");
            return this->node_->data; 
        };
    };

    class Iterator : public ConstIterator {
    public:
        Iterator() : ConstIterator() {}

    }
};

#endif  // SRC_LIST_H_