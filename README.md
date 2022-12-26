# s21_containers

Implementation of the s21_containers.h. library.

The russian version of the task can be found in the repository.

## Introduction

As part of the project you need to write your own library that implements the basic standard C++ container classes: `list`, `map`, `queue`, `set`, `stack` and `vector`. Implementations should provide a full set of standard methods and attributes for element handling, container capacity checking and iteration. As a bonus, you can also implement several other container classes from the C++ container library that are not as commonly used, but differ in their implementation details.

## Information

For most people, the word «container» is self-explanatory and comes from the English word: contain. Same in programming: containers are used to contain sets of objects of the same type i.e elements. However, there are a huge number of container classes. This is because container classes differ in the organisation of stored object sets and in the methods provided to interact with them. So, for example, lists (`list`) store any object, while sets (`set`) store only some unique objects.

The very need to separate containers, rather than using the same one for different tasks, stems not only from obvious functional differences. In some cases, it is more efficient to use lists, e.g. when the task at hand requires frequent insertion of elements in the middle of the container, but when adding new elements at the end only, it is better to use a queue.

Each type of containers should provide the user with the following methods:

- standard constructors (default constructor, copy constructor, move constructor, constructor with initialization list, see materials);

- methods for accessing container elements (e.g. accessing an element with the index i);

- methods for checking if a container is full (e.g. the number of elements in the container, check if the container is empty);

- methods for changing the container (removing and adding new elements, cleaning the container);

- methods for dealing with the container iterator.

Iterators provide an access to container elements. The specific type of iterator will be different for each container. This is because of the different kind of object set organisation in container classes, as well as the actual implementation of the container. Iterators are implemented to work in a similar way that a pointer to an array element in C does. So, this approach with iterators allows interaction with any containers in the same way. Containers provide iterators via the `begin()` and `end()` methods, which point to the first and next after the last elements of the container respectively.

Iterator `iter` has the following operations:

- `*iter`: gets the element pointed to by the iterator;

- `++iter`: moves the iterator forward to the next element

- `--iter`: moves the iterator backwards to the previous element;

- `iter1 == iter2`: two iterators are equal if they point to the same element

- `iter1 != iter2`: two iterators are not equal if they point to different elements

Besides the special organisation of objects and the provision of necessary methods, the implementation of container classes requires the templating of objects.

Template classes or class templates are used when you want to create a class that depends on additional external parameters, which can be other classes or data types. For example, if you need to create a list class, you’ll want to avoid rewriting the list implementation for all possible element types. It would be nice to write one class with a parameter and get several specific list classes at once (character, integer, floating-point, user-defined types lists, etc.).
In C++, containers, along with iterators and some algorithms, are part of the Standard Template Library (STL) for this very reason.

There are two main types of containers: sequence and associative containers. To find an element in sequence containers (`list`, `vector`, `array`, `stack`, `queue`), you have to look through the container one by one, while in associative containers (`map`, `set`, `multiset`) you just need to look through the key associated with the value.


## Chapter III

- The program must be developed in C++ language of C++17 standard using gcc compiler
- The program code must be located in the src folder
- When writing code it is necessary to follow the Google style
- Make sure to use iterators
- Classes must be template
- Classes must be implemented within the `s21` namespace
- Prepare full coverage of container classes methods with unit-tests using the GTest library
- Copying of the Standard Template Library (STL) implementation is not allowed
- The logic of the Standard Template Library (STL) must be followed (in terms of checks, memory handling and behaviour in abnormal situations)

### Part 1. Implementation of the s21_containers.h library

You need to implement the `s21_containers.h` library classes (specifications are given in the relevant material sections, see **"Main containers"**). \
List of classes: `list`, `map`, `queue`, `set`, `stack`, `vector`.
- Make it as a header file `s21_containers.h` which includes different header files with implementations of the specified containers (`s21_list.h`, `s21_map.h` and etc.) 
- Provide a Makefile for testing the library (with targets clean, test)
- The classical implementation of containers should be considered as a basis, but the final choice of implementations remains free. Except for the list - it should be implemented via the list structure rather than the array

*Tip*: You can move the same implementation of container methods to base classes. For example, for a queue and a stack, or for a list and a vector. There is a UML diagram of the STL library in materials as *one possible example* of hierarchical construction. However, your implementation does not have to be strictly tied to this UML diagram.

### Part 2. Bonus. Implementation of the s21_containersplus.h library.

You need to implement the `s21_containersplus.h` library functions (see **"Additional containers"** for specifications). \
List of classes to be implemented additionally: `array`, `multiset`.
- Make it as a header file `s21_containersplus.h` which includes different header files with implementations of the specified containers (`s21_array.h`, `s21_multiset.h`) 
- Provide a Makefile for testing the library (with targets clean, test)
- The classical implementation of containers should be considered as a basis, but the final choice of the algorithm remains free.

### Part 3. Bonus. Implementation of the modified `emplace` methods

You need to complete the classes with the appropriate methods, according to the table:

| Modifiers      | Definition                                      | Containers |
|----------------|-------------------------------------------------| -------------------------------------------|
| `iterator emplace(const_iterator pos, Args&&... args)`          | inserts new elements into the container directly before `pos`  | List, Vector |
| `void emplace_back(Args&&... args)`          | appends new elements to the end of the container  | List, Vector, Queue |
| `void emplace_front(Args&&... args)`          | appends new elements to the top of the container  | List, Stack |
| `vector<std::pair<iterator,bool>> emplace(Args&&... args)`          | inserts new elements into the container  | Map, Set, Multiset |

Note: the arguments are the already created elements that should be inserted into the appropriate container.

*Tip 1*: notice that each of these methods uses an Args&&... args - Parameter pack construct. This construct allows a variable number of parameters to be passed to a function or method. So, when calling a method defined as `iterator emplace(const_iterator pos, Args&&... args)`, you can write either `emplace(pos, arg1, arg2)` or `emplace(pos, arg1, arg2, arg3)`.

*Tip 2*: remember to test methods for different cases, including boundary ones.

