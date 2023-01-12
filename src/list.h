#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#include <iostream>
using namespace std;

template <typename T>
class s21_list {
    T* arr;
    int size_;
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

    Iterator begin() { return arr; }
    Iterator end() { return arr+size_; }

    // friend ostream& operator<< (ostream& s, const s21_list<T>& size);

    class Iterator {
        T* cur;
    public:
        Iterator(T* first) : cur(first) {}

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