#ifndef S21_CONTAINERS_S21_CONTAINERS_S21_VECTOR_H_
#define S21_CONTAINERS_S21_CONTAINERS_S21_VECTOR_H_

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <utility>

namespace s21 {

template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

 private:
  size_type size_ = 0;
  size_type capacity_ = 0;
  iterator buffer_ = nullptr;

 public:
  vector() : size_(0), capacity_(0), buffer_(nullptr) {}

  vector(size_type size) {
    this->size_ = size;
    capacity_ = size;
    buffer_ = new value_type[size_];
    if (size > 0) {
      buffer_ = new value_type[capacity_];
    }
  }

  vector(std::initializer_list<value_type> const &init) : vector(init.size()) {
    std::copy(init.begin(), init.end(), buffer_);
  }

  vector(const vector &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    buffer_ = nullptr;
    if (size_ > 0) {
      buffer_ = new value_type[capacity_];
    }
    std::copy(other.begin(), other.end(), buffer_);
  }

  vector(vector &&other)
      : size_(other.size_), capacity_(other.capacity_), buffer_(other.buffer_) {
    other.buffer_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ~vector() {
    if (buffer_) {
      delete[] buffer_;
    }
    size_ = 0;
    capacity_ = 0;
    buffer_ = nullptr;
  }

  vector &operator=(vector &&other) {
    if (this != &other) {
      buffer_ = other.buffer_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.buffer_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

 public:
  // Vector Element access
  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("at The index is out of range");
    return buffer_[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size_) throw std::out_of_range("at The index is out of range");
    return buffer_[pos];
  }

  reference operator[](size_type pos) { return at(pos); }

  const_reference operator[](size_type pos) const { return at(pos); }

  reference front() {
    if (size_ == 0) throw std::out_of_range("the vector is empty");
    return *begin();
  }

  const_reference front() const {
    if (size_ == 0) throw std::out_of_range("the vector is empty");
    return *begin();
  }

  reference back() {
    if (size_ == 0) throw std::out_of_range("the vector is empty");
    return buffer_[size_ - 1];
  }

  const_reference back() const {
    if (size_ == 0) throw std::out_of_range("the vector is empty");
    return buffer_[size_ - 1];
  }

  iterator data() { return buffer_; }

  const_iterator data() const { return buffer_; }

  // Vector Iterators
  iterator begin() { return buffer_; }

  const_iterator begin() const { return buffer_; }

  iterator end() { return buffer_ + size_; }

  const_iterator end() const { return buffer_ + size_; }

 public:
  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }

  size_type max_size() const {
    // Возвращаем максимально возможное количество элементов в векторе
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  // выделяет хранилище элементов размера и копирует текущие элементы массива в новый выделенный массив
  void reserve(size_type size) {
    if (size > max_size())
      throw std::length_error("The size of the vector cannot exceed the maximum size");
    if (size > capacity()) {
        value_type *new_data = new value_type[size];
        for (size_type i = 0; i < size_; i++) {
            new_data[i] = std::move(buffer_[i]);
        }
        delete[] buffer_;
        buffer_ = new_data;
        capacity_ = size;
    }
  }

  // возвращает количество элементов, которые могут храниться в выделенной в данный момент памяти
  size_type capacity() const { return capacity_; }

  void clear() { 
    for (size_type i = 0; i < size_; ++i) {
        buffer_[i].~value_type(); // вызов деструкторов для элементов
    }
    size_ = 0; 
  }

  // вставляет элемент со значением value перед итератором pos
  iterator insert(const_iterator pos, value_type &&value) {
    size_type index = pos - buffer_; // индекс вставляемого элемента
    if (index > size_)
      throw std::out_of_range("going beyond the dimensions of the vector");
    if (size_ == capacity_) { // размер вектора равен ёмкости
      reserve(2 * capacity_); // выделяется новая память с удвоенной ёмкостью
      // Удвоение памяти используется для того, чтобы уменьшить количество перераспределений, 
      // которые могут быть очень дорогостоящими в случае, если размер контейнера увеличивается поштучно
    }
    for (size_type i = size_; i > index; --i) {
      buffer_[i] = std::move(buffer_[i - 1]); 
      // Сдвиг элементов вектора с индексами от index до size_ (исключая index) на одну позицию вправо.
    }
    buffer_[index] = std::move(value); // Присвоение элементу с индексом index значения, передаваемого в функцию
    ++size_; // Увеличение размера вектора
    return iterator(buffer_ + index); // Возврат итератора на элемент, который был вставлен
  }

  // стирает элемент в позиции
  iterator erase(const_iterator pos) {
    size_type index = pos - buffer_; // индекс элемента в векторе, который нужно удалить
    if (index >= size_) // индекс больше или равен размеру вектора
      throw std::out_of_range("index out of range");
    for (size_type i = index; i < size_ - 1; ++i) { // перебираем элементы вектора, начиная с удаленного элемента
      buffer_[i] = std::move(buffer_[i + 1]); // перемещаем их на одну позицию влево
    }
    --size_; // уменьшаем размер вектора
    return iterator(buffer_ + index); // возвращаем итератор на удаленный элемент
  }

  // метод добавления элемента в конец вектора
  void push_back(const_reference value) {
    if (size_ == capacity_) { // Если количество элементов в векторе равно емкости => не хватает места
      reserve(2 * capacity_); // увеличиваем capacity в два раза, если не хватает места
    }
    buffer_[size_++] = value; // добавляем элемент в конец и увеличиваем size_
  }

  void pop_back() {
    if (size_ == 0) {
        throw std::out_of_range("vector is empty");
    }
    --size_;
  }

  void swap(vector& other) {
    // Swap capacity
    size_type tmp_cap = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = tmp_cap;

    // Swap size
    size_type tmp_size = size_;
    size_ = other.size_;
    other.size_ = tmp_size;

    // Swap buffer
    iterator tmp_buf = buffer_;
    buffer_ = other.buffer_;
    other.buffer_ = tmp_buf;
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    size_type index = pos - buffer_;
    if (index > size_ + 1)
      throw std::out_of_range("going beyond the dimensions of the vector");
    if (size_ == capacity_) {
      reserve(2 * capacity_);
    }
    iterator it = begin() + index;
    insert(it, std::forward<Args>(args)...);
    return it;
  }

  template <typename... Args>
  iterator emplace_back(Args &&... args) {
    for (auto &&item : {std::forward<Args>(args)...}) { // Итерируемся по списку аргументов
      push_back(item); // Добавляем элементы в конец вектора
    }
    return end() - 1; // Возвращаем итератор на последний добавленный элемент
  }
};

}  // namespace s21

#endif  // S21_CONTAINERS_S21_CONTAINERS_S21_VECTOR_H_