#ifndef S21_CONTAINERS_S21_CONTAINERS_S21_MAP_H_
#define S21_CONTAINERS_S21_CONTAINERS_S21_MAP_H_

#include <stdexcept>

#include "s21_tree.h"

namespace s21 {
template <class Key, class Type>
class map {
 public:
  using key_type = Key;
  using mapped_type = Type;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;

  struct MapValueComparator {
    bool operator()(const_reference value1,
                    const_reference value2) const noexcept {
      return value1.first < value2.first;
    }
  };

  using tree_type = RedBlackTree<value_type, MapValueComparator>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

  map() : tree_(new tree_type{}) {}

  map(std::initializer_list<value_type> const &items) : map() {
    for (auto item : items) {
      insert(item);
    }
  }

  map(const map &other) : tree_(new tree_type(*other.tree_)) {}

  map(map &&other) noexcept : tree_(new tree_type(std::move(*other.tree_))) {}

  map &operator=(const map &other) {
    *tree_ = *other.tree_;
    return *this;
  }

  map &operator=(map &&other) noexcept {
    *tree_ = std::move(*other.tree_);
    return *this;
  }

  ~map() {
    delete tree_;
    tree_ = nullptr;
  }

  mapped_type &at(const key_type &key) {
    value_type search_pair(key, mapped_type{});
    iterator it_search = tree_->Find(search_pair);

    if (it_search == end()) {
      throw std::out_of_range(
          "s21::map::at: No element exists with key equivalent to key");
    } else {
      return (*it_search).second;
    }
  }

  const mapped_type &at(const key_type &key) const {
    return const_cast<map<Key, Type> *>(this)->at(key);
  }

  mapped_type &operator[](const key_type &key) {
    value_type search_pair(key, mapped_type{});
    iterator it_search = tree_->Find(search_pair);

    if (it_search == end()) {
      std::pair<iterator, bool> result = tree_->InsertUnique(search_pair);
      return (*result.first).second;
    } else {
      return (*it_search).second;
    }
  }

  iterator begin() noexcept { return tree_->Begin(); }

  const_iterator begin() const noexcept { return tree_->Begin(); }

  iterator end() noexcept { return tree_->End(); }

  const_iterator end() const noexcept { return tree_->End(); }

  bool empty() const noexcept { return tree_->Empty(); }

  size_type size() const noexcept { return tree_->Size(); }

  size_type max_size() const noexcept { return tree_->MaxSize(); }

  void clear() noexcept { tree_->Clear(); }

  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_->InsertUnique(value);
  }

  std::pair<iterator, bool> insert(const key_type &key,
                                   const mapped_type &obj) {
    return tree_->InsertUnique(value_type{key, obj});
  }

  std::pair<iterator, bool> insert_or_assign(const key_type &key,
                                             const mapped_type &obj) {
    iterator result = tree_->Find(value_type{key, obj});

    if (result == end()) {
      return tree_->InsertUnique(value_type{key, obj});
    }

    (*result).second = obj;

    return {result, false};
  }

  void erase(iterator pos) noexcept { tree_->Erase(pos); }

  void swap(map &other) noexcept { tree_->Swap(*other.tree_); }

  void merge(map &other) noexcept { tree_->MergeUnique(*other.tree_); }

  bool contains(const key_type &key) const noexcept {
    value_type search_pair(key, mapped_type{});
    iterator it_search = tree_->Find(search_pair);
    return !(it_search == end());
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&... args) {
    return tree_->EmplaceUnique(std::forward<Args>(args)...);
  }

 private:
  tree_type *tree_;
};

}  // namespace s21

#endif  // S21_CONTAINERS_S21_CONTAINERS_S21_MAP_H_