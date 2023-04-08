#ifndef CONTAINERS_S21_ARRAY_H
#define CONTAINERS_S21_ARRAY_H

#include <initializer_list>

namespace s21 {
template <typename T, std::size_t V>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = std::size_t;

 private:
  value_type data_[V];

 public:
  array() noexcept = default;

  explicit array(std::initializer_list<value_type> const &items) {
    if (items.size() > V) {
      throw std::logic_error("too many initializers");
    }

    for (size_type i = 0; i < V; ++i) {
      data_[i] = items.begin()[i];
    }
  };

  explicit array(const array &a) {
    for (size_type i = 0; i < V; ++i) {
      data_[i] = a[i];
    }
  }

  explicit array(array &&a) noexcept {
    auto iter = data_;
    for (auto &item : a) {
      *(iter++) = std::move(item);
    }
  };

  array &operator=(array &&a) noexcept {
    for (size_type i = 0; i < V; ++i) {
      data_[i] = std::move(a[i]);
    }
    return *this;
  };

  ~array() = default;

  reference at(size_type pos) {
    if (pos >= V) {
      throw std::out_of_range("Out of range");
    }
    return data_[pos];
  }

  constexpr reference operator[](size_type pos) { return data_[pos]; }

  constexpr const_reference operator[](size_type pos) const {
    return data_[pos];
  }

  constexpr const_reference front() const { return *data_; };

  constexpr const_reference back() const { return data_[V - 1]; };

  constexpr iterator data() noexcept { return data_; };

  constexpr iterator begin() noexcept { return data_; };

  constexpr iterator end() noexcept { return data_ + V; };

  constexpr bool empty() const noexcept { return V == 0; };

  constexpr size_type size() const noexcept { return V; };

  constexpr size_type max_size() const noexcept { return V; };

  constexpr void swap(array &other) noexcept {
    for (size_type i = 0; i < V; ++i) {
      std::swap(data_[i], other.data_[i]);
    }
  }

  void fill(const_reference value) {
    for (auto &item : data_) {
      item = value;
    }
  }
};

}  // namespace s21

#endif  // CONTAINERS_S21_ARRAY_H