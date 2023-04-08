#ifndef CONTAINERS_S21_QUEUE_H_
#define CONTAINERS_S21_QUEUE_H_

#include <algorithm>
#include <initializer_list>

#include "s21_list.h"

namespace s21 {

template <typename T>
class queue : protected s21::list<T> {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using inheritance = s21::list<T>;

 public:
  using inheritance::inheritance;

 public:
  reference front() noexcept { return inheritance::front(); }

  const_reference front() const noexcept { return inheritance::front(); }

  reference back() noexcept { return inheritance::back(); }

  const_reference back() const noexcept { return inheritance::back(); }

 public:
  bool empty() const noexcept { return inheritance::empty(); }

  size_type size() const noexcept { return inheritance::size(); }

  void push(const_reference value) { inheritance::push_back(value); }

  void push(value_type &&value) { inheritance::push_back(std::move(value)); }

  void pop() noexcept { inheritance::pop_front(); }

  void swap(queue &s) noexcept {
    inheritance::swap(static_cast<inheritance &>(s));
  }

  template <typename... Args>
  void emplace_back(Args &&... args) {
    inheritance::emplace_back(std::forward<Args>(args)...);
  }
};

}  // namespace s21

#endif  // CONTAINERS_S21_QUEUE_H_