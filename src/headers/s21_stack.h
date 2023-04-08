#ifndef S21_CONTAINERS_S21_STACK_H_
#define S21_CONTAINERS_S21_STACK_H_

#include <algorithm>

#include "s21_list.h"

namespace s21 {

template <typename T>
class stack {
 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;

  stack() { data_.clear(); }

  stack(std::initializer_list<value_type> const &items) {
    data_.clear();
    for (const auto &i : items) {
      push(i);
    }
  }

  stack(const stack &st) { operator=(st); }

  stack(stack &&st) { operator=(std::move(st)); }

  ~stack() { data_.clear(); }

  stack &operator=(stack &&st) {
    this->data_ = std::move(st.data_);
    return *this;
  }

  stack &operator=(const stack &st) {
    this->data_ = st.data_;
    return *this;
  }

  const_reference top() { return data_.front(); }

  bool empty() { return data_.empty(); }
  size_type size() { return data_.size(); }

  void push(const_reference value) { data_.push_front(value); }
  void pop() { data_.pop_front(); }
  void swap(stack &other) { data_.swap(other.data_); }

  template <typename... Args>
  void emplace_front(Args &&... args) {
    return data_.emplace_back(std::forward<Args>(args)...);
  }

 private:
  list<value_type> data_;
};

}  // namespace s21

#endif  // S21_CONTAINERS_S21_STACK_H_
