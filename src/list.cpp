#include "list.h"

using namespace std;

// list<int> myList = {1, 2, 3, 4, 5, 6};
// list<int>::iterator it;

int main() {
    list<int> arr(3);
    auto it = list<int>::iterator(arr.begin());
    arr[it] = 1;
    arr[it++] = 2;
    arr[it++] = 3;

    

    cout << "2: " << it + 2 << endl;

}

