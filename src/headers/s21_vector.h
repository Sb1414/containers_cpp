#ifndef SRC_S21_VECTOR_H_
#define SRC_S21_VECTOR_H_
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

  // выделяет хранилище элементов размера и копирует текущие элементы массива в
  // новый выделенный массив
  void reserve(size_type size) {
    if (size > max_size())
      throw std::length_error(
          "The size of the vector cannot exceed the maximum size");
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

  // возвращает количество элементов, которые могут храниться в выделенной в
  // данный момент памяти
  size_type capacity() const { return capacity_; }

  void shrink_to_fit() {
    if (size_ == capacity_) {
      return;
    }
    value_type *new_buffer = new value_type[size_];
    for (size_type i = 0; i < size_; ++i) {
      new (new_buffer + i) value_type(std::move(buffer_[i]));
    }
    std::swap(buffer_, new_buffer);
    capacity_ = size_;
    delete[] new_buffer;
  }

  void clear() {
    for (size_type i = 0; i < size_; ++i) {
      buffer_[i].~value_type();  // вызов деструкторов для элементов
    }
    size_ = 0;
  }

  iterator insert(const_iterator pos, value_type &&value) {
    size_type index = pos - buffer_;
    if (index > size_)
      throw std::out_of_range("going beyond the dimensions of the vector");
    if (size_ == capacity_) {
      reserve(2 * capacity_);
    }
    for (size_type i = size_; i > index; --i) {
      buffer_[i] = std::move(buffer_[i - 1]);
    }
    buffer_[index] = std::move(value);
    ++size_;
    return iterator(buffer_ + index);
  }

  iterator erase(const_iterator pos) {
    size_type index = pos - buffer_;
    if (index >= size_) throw std::out_of_range("index out of range");
    for (size_type i = index; i < size_ - 1; ++i) {
      buffer_[i] = std::move(buffer_[i + 1]);
    }
    --size_;
    return iterator(buffer_ + index);
  }

  void push_back(const_reference value) {
    if (size_ == capacity_) {
      reserve(2 * capacity_);
    }
    buffer_[size_++] = value;
  }

  void pop_back() {
    if (size_ == 0) {
      throw std::out_of_range("vector is empty");
    }
    --size_;
  }

  void swap(vector &other) {
    size_type tmp_cap = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = tmp_cap;

    size_type tmp_size = size_;
    size_ = other.size_;
    other.size_ = tmp_size;

    iterator tmp_buf = buffer_;
    buffer_ = other.buffer_;
    other.buffer_ = tmp_buf;
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args &&...args) {
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
  iterator emplace_back(Args &&...args) {
    for (auto &&item : {std::forward<Args>(args)...}) {
      push_back(item);
    }
    return end() - 1;
  }
};

}  // namespace s21

#endif  // SRC_S21_VECTOR_H_