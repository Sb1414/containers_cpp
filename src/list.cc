#include <iostream>
#include <list>

using namespace std;

// list<int> myList = {1, 2, 3, 4, 5, 6};
// list<int>::iterator it;

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

int main() {
    s21_list<int> arr(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    auto it = s21_list<int>::Iterator(arr.begin());

    cout << "2: " << it + 2 << endl;

}

