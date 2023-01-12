#include "list.h"

using namespace std;

// list<int> myList = {1, 2, 3, 4, 5, 6};
// list<int>::iterator it;

int main() {
    s21_list<int> arr(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    auto it = s21_list<int>::Iterator(arr.begin());

    cout << "2: " << it + 2 << endl;

}

