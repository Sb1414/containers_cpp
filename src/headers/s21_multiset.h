#ifndef CONTAINERS_S21_MULTISET_H_
#define CONTAINERS_S21_MULTISET_H_

#include "s21_tree.h"

namespace s21 {
template <class Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = key_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using tree_type = tree<value_type>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

  multiset() : tree_(new tree_type{}) {}

  multiset(std::initializer_list<value_type> const &items) : multiset() {
    for (auto item : items) {
      insert(item);
    }
  }

  multiset(const multiset &other) : tree_(new tree_type(*other.tree_)) {}

  multiset(multiset &&other) noexcept
      : tree_(new tree_type(std::move(*other.tree_))) {}

  multiset &operator=(const multiset &other) {
    *tree_ = *other.tree_;
    return *this;
  }

  multiset &operator=(multiset &&other) noexcept {
    *tree_ = std::move(*other.tree_);
    return *this;
  }

  ~multiset() {
    delete tree_;
    tree_ = nullptr;
  }

  iterator begin() noexcept { return tree_->Begin(); }

  const_iterator begin() const noexcept { return tree_->Begin(); }

  iterator end() noexcept { return tree_->End(); }

  const_iterator end() const noexcept { return tree_->End(); }

  bool empty() const noexcept { return tree_->Empty(); }

  size_type size() const noexcept { return tree_->Size(); }

  size_type max_size() const noexcept { return tree_->MaxSize(); }

  void clear() noexcept { tree_->Clear(); }

  iterator insert(const value_type &value) { return tree_->Insert(value); }

  void erase(iterator pos) noexcept { tree_->Erase(pos); }

  void swap(multiset &other) noexcept { tree_->Swap(*other.tree_); }

  void merge(multiset &other) noexcept { tree_->Merge(*other.tree_); }

  size_type count(const key_type &key) const {
    auto lower_iterator = lower_bound(key);
    if (*lower_iterator != key) {
      return 0;
    }

    auto end_iterator = end();
    size_type result_count = 0;
    while (lower_iterator != end_iterator && *lower_iterator == key) {
      ++result_count;
      ++lower_iterator;
    }

    return result_count;
  }

  iterator find(const key_type &key) noexcept { return tree_->Find(key); }

  const_iterator find(const key_type &key) const noexcept {
    return tree_->Find(key);
  }

  bool contains(const key_type &key) const noexcept {
    return tree_->Find(key) != tree_->End();
  }

  std::pair<iterator, iterator> equal_range(const key_type &key) noexcept {
    iterator first = lower_bound(key);
    iterator second = upper_bound(key);
    return std::pair<iterator, iterator>{first, second};
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type &key) const noexcept {
    const_iterator first = lower_bound(key);
    const_iterator second = upper_bound(key);

    return std::pair<const_iterator, const_iterator>{first, second};
  }

  iterator lower_bound(const key_type &key) noexcept {
    return tree_->LowerBound(key);
  }

  const_iterator lower_bound(const key_type &key) const {
    return tree_->LowerBound(key);
  }

  iterator upper_bound(const key_type &key) noexcept {
    return tree_->UpperBound(key);
  }

  const_iterator upper_bound(const key_type &key) const noexcept {
    return tree_->UpperBound(key);
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&...args) {
    return tree_->Emplace(std::forward<Args>(args)...);
  }

 private:
  tree_type *tree_;
};
}  // namespace s21

#endif  // CONTAINERS_S21_MULTISET_H_
