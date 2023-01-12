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

    T* arr;
    int size_;

public:
    using Node = ListNode;

private:
    size_type _size;
    Node* _front;
    Node* _back;

public:
    class Iterator;
    s21_list(int size = 1) : size_(size) {
        arr = new T[size];
    }

    T& operator[] (const int& size) {
        if (size > 0 && size < size_)
            return arr[size];
        return arr[0];
    }

    Iterator begin() { return Iterator(_front); }
    Iterator end() { return Iterator(_front + _size); }

    // Iterator begin() { return arr; }
    // Iterator end() { return arr+size_; }

    // friend ostream& operator<< (ostream& s, const s21_list<T>& size);

    class Iterator {
        Node* data_;
        pointer _node;
        // T* cur;
    public:
        // Iterator(T* first) : cur(first) {}

        Iterator() = default;
        explicit Iterator(pointer ptr) { _node = ptr; }
        value_type& operator*() const { return _node->data; }
        pointer operator->() { return _node; }

        Iterator& operator++() {
            _node = _node->next;
            return *this;
        }

        T& operator+ (int n) { return *(cur + n); }
        T& operator- (int n) { return *(cur - n); }

        T& operator++ (int) { return *cur++; }
        T& operator-- (int) { return *cur--; }
        T& operator++ () { return *++cur; }
        T& operator-- () { return *--cur; }

        bool operator!= (const Iterator& it) { return cur != it.cur; }
        bool operator== (const Iterator& it) { return cur == it.cur; }
        T& operator* () { return *cur; };
    };
};

#endif  // SRC_LIST_H_