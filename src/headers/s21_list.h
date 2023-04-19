#ifndef CONTAINERS_S21_LIST_H
#define CONTAINERS_S21_LIST_H

namespace s21 {
template <typename Type>
class list {
 private:
  class ListNode;

 public:
  class ListIterator;
  class ListIteratorConst;
  using value_type = Type;
  using reference = Type &;
  using const_reference = const Type &;
  using iterator = ListIterator;
  using const_iterator = ListIteratorConst;
  using size_type = std::size_t;
  using node_type = ListNode;
  using node_pointer = ListNode *;

 private:
  // узел двусвязного списка
  class ListNode {
   public:
    ListNode() noexcept : next_(this), prev_(this), value_(value_type{}) {}

    explicit ListNode(const_reference value) noexcept
        : next_(nullptr), prev_(nullptr), value_(value) {}

    explicit ListNode(value_type &&value) noexcept
        : next_(nullptr), prev_(nullptr), value_(std::move(value)) {}

    void SwapValues(node_type *other_node) noexcept {
      std::swap(value_, other_node->value_);
    }

    void SwapNextPrev() noexcept { std::swap(next_, prev_); }

    node_type *next_;   // указатель на следующий узел
    node_type *prev_;   // указатель на предыдущий узел
    value_type value_;  // значение текущего узла
  };

 public:
  class ListIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = list::value_type;
    using pointer = value_type *;
    using reference = value_type &;

    ListIterator() = delete;

    explicit ListIterator(node_type *node) noexcept : node_(node) {}

    reference operator*() const noexcept { return node_->value_; }

    iterator &operator++() {
      node_ = node_->next_;
      return *this;
    }

    iterator operator++(int) {
      iterator it = *this;
      ++(*this);
      return it;
    }

    iterator &operator--() {
      node_ = node_->prev_;
      return *this;
    }

    iterator operator--(int) {
      iterator it = *this;
      --(*this);
      return it;
    }

    bool operator==(const iterator &other) const {
      return node_ == other.node_;
    }

    bool operator!=(const iterator &other) const { return !(*this == other); }

    node_type *node_;
  };

  class ListIteratorConst {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = list::value_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    ListIteratorConst() : node_(nullptr) {}

    explicit ListIteratorConst(const node_type *node) : node_(node) {}

    ListIteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->value_; }

    const_iterator &operator++() {
      node_ = node_->next_;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() {
      node_ = node_->prev_;
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) {
      return !(it1 == it2);
    }

    const node_type *node_;
  };

  list() : head_(new node_type{}), tail_(new node_type{}), size_(0) {}

  explicit list(size_type n) : list() {
    while (n > 0) {
      push_back(value_type{});
      --n;
    }
  }

  list(std::initializer_list<value_type> const &items) : list() {
    for (auto item : items) {
      push_back(item);
    }
  }

  list(const list &other) : list() {
    for (auto list_element : other) {
      push_back(list_element);
    }
  }

  list(list &&other)
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
  }

  list &operator=(const list &other) {
    if (this != &other) {
      clear();
      for (const auto &item : other) {
        push_back(item);
      }
    }
    return *this;
  }

  list &operator=(list &&other) {
    if (this != &other) {
      clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  ~list() {
    clear();
    delete head_;
    head_ = nullptr;
  }

  reference front() noexcept { return *begin(); }
  const_reference front() const noexcept { return *begin(); }
  reference back() noexcept { return *std::prev(end()); }
  const_reference back() const noexcept { return *std::prev(end()); }
  iterator begin() noexcept { return iterator{head_->next_}; }
  const_iterator begin() const noexcept { return const_iterator{head_->next_}; }
  iterator end() noexcept { return iterator{head_}; }
  const_iterator end() const noexcept { return const_iterator{head_}; }

  bool empty() const noexcept { return size_ == 0; }

  size_type size() const noexcept { return size_; };

  size_type max_size() const noexcept {
    return ((std::numeric_limits<size_type>::max() / 2) / sizeof(node_type)) -
           1;
  }

  iterator insert(iterator pos, const_reference value) {
    node_pointer new_node = new ListNode(value);
    node_pointer next_node = pos.node_;
    node_pointer prev_node = next_node->prev_;

    prev_node->next_ = new_node;
    new_node->prev_ = prev_node;
    new_node->next_ = next_node;
    next_node->prev_ = new_node;
    ++size_;

    return iterator(new_node);
  }

  const_iterator insert(const_iterator pos, const_reference value) const {
    return const_cast<list *>(this)->insert(pos, value);
  }

  template <typename InputIterator>
  void insert(const_iterator pos, InputIterator first, InputIterator last) {
    for (auto it = first; it != last; ++it) {
      insert(pos, *it);
    }
  }

  void erase(iterator pos) noexcept {
    if (pos != end()) {
      pos.node_->prev_->next_ = pos.node_->next_;
      pos.node_->next_->prev_ = pos.node_->prev_;
      delete pos.node_;
      --size_;
    }
  }

  void clear() noexcept {
    while (size_ > 0) {
      erase(begin());
    }
  }

  void push_back(const_reference value) { insert(end(), value); }

  void pop_back() noexcept { erase(--end()); }

  void push_front(const_reference value) {
    node_pointer new_node = new ListNode(value);
    new_node->next_ = head_->next_;
    new_node->prev_ = head_;
    head_->next_->prev_ = new_node;
    head_->next_ = new_node;
    ++size_;
  }

  void pop_front() {
    if (empty()) {
      return;
    }
    node_pointer node_to_delete = head_->next_;
    head_->next_ = node_to_delete->next_;
    node_to_delete->next_->prev_ = head_;
    delete node_to_delete;
    --size_;
  }

  void swap(list &other) {
    if (this == &other) {
      return;
    }
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
  }

  void splice(const_iterator pos, list &other) noexcept {
    if (!other.empty()) {
      iterator it_current{const_cast<node_type *>(pos.node_)};
      iterator it_other = other.end();

      it_other.node_->next_->prev_ = it_current.node_->prev_;
      it_other.node_->prev_->next_ = it_current.node_;

      it_current.node_->prev_->next_ = it_other.node_->next_;
      it_current.node_->prev_ = it_other.node_->prev_;

      size_ += other.size();

      other.size_ = 0;
      other.head_->next_ = other.head_;
      other.head_->prev_ = other.head_;
    }
  }

  void merge(list &other) {
    auto it1 = begin();
    auto it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
      if (*it1 < *it2) {
        ++it1;
      } else {
        insert(it1, *it2);
        ++it2;
      }
    }

    while (it2 != other.end()) {
      push_back(*it2);
      ++it2;
    }

    other.clear();
  }

  void reverse() noexcept {
    auto begin_iterator = begin();
    auto end_iterator = end();
    while (begin_iterator != end_iterator) {
      begin_iterator.node_->SwapNextPrev();
      --begin_iterator;
    }
    head_->SwapNextPrev();
  }

  void unique() {
    iterator begin_it = begin();
    iterator end_it = end();
    iterator prev_it = begin_it;
    ++begin_it;
    for (; begin_it != end_it; ++begin_it) {
      if (*begin_it == *prev_it) {
        erase(begin_it);
        begin_it = prev_it;
      } else {
        ++prev_it;
      }
    }
  }

  void sort() {
    if (size() < 2) {
      return;
    }
    auto it = begin();
    auto end_it = end();
    while (it != end_it) {
      auto current = *it;
      auto next_it = std::next(it);
      auto insert_it = it;
      while (insert_it != begin() && *std::prev(insert_it) > current) {
        --insert_it;
      }
      if (insert_it == it) {
        it = next_it;
        continue;
      }
      erase(it);
      insert(insert_it, current);
      it = next_it;
    }
  }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PART 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  template <typename... Args>
  iterator emplace(const_iterator pos, Args &&...args) noexcept {
    node_type *new_node;
    iterator it_{const_cast<node_type *>(pos.node_)};
    for (auto item : {std::forward<Args>(args)...}) {
      new_node = new node_type(std::move(item));
      new_node->next_ = it_.node_;
      new_node->prev_ = it_.node_->prev_;
      it_.node_->prev_->next_ = new_node;
      it_.node_->prev_ = new_node;
      ++size_;
    }
    return iterator(new_node);
  }

  template <typename... Args>
  void emplace_back(Args &&...args) {
    emplace(end(), std::forward<Args>(args)...);
  }

  template <typename... Args>
  void emplace_front(Args &&...args) {
    emplace(begin(), std::forward<Args>(args)...);
  }

 private:
  node_type *head_;
  node_type *tail_;
  size_type size_;
};

}  // namespace s21

#endif  // CONTAINERS_S21_LIST_H