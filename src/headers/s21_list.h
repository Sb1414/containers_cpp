#ifndef SRC_S21_LIST_H
#define SRC_S21_LIST_H

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

    node_type *next_;
    node_type *prev_;
    value_type value_;
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

    iterator &operator++() noexcept {
      node_ = node_->next_;
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    iterator &operator--() noexcept {
      node_ = node_->prev_;
      return *this;
    }

    iterator operator--(int) noexcept {
      iterator tmp{node_};
      --(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const iterator &other) const noexcept {
      return node_ != other.node_;
    }

    node_type *node_;
  };

  class ListIteratorConst {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = list::value_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    ListIteratorConst() = delete;

    explicit ListIteratorConst(const node_type *node) : node_(node) {}

    ListIteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->value_; }

    const_iterator &operator++() noexcept {
      node_ = node_->next_;
      return *this;
    }

    const_iterator operator++(int) noexcept {
      const_iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() noexcept {
      node_ = node_->prev_;
      return *this;
    }

    const_iterator operator--(int) noexcept {
      const_iterator tmp{node_};
      --(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ != it2.node_;
    }

    const node_type *node_;
  };

  list() : head_(new node_type{}), size_(0U) {}

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

  list(list &&other) noexcept : list() { splice(begin(), other); }

  list &operator=(const list &other) {
    if (this == &other) {
      return *this;
    }
    clear();
    for (const auto &elem : other) {
      push_back(elem);
    }
    return *this;
  }

  list &operator=(list &&other) noexcept {
    if (this != &other) {
      clear();
      splice(begin(), other);
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
    if (empty()) {
      push_back(value);
    } else {
      insert(begin(), value);
    }
  }

  void pop_front() {
    if (!empty()) {
      erase(begin());
    }
  }

  void swap(list &other) noexcept {
    if (this != &other) {
      std::swap(head_, other.head_);
      std::swap(size_, other.size_);
    }
  }

  void merge(list &other) {
    if (size_ + other.size_ > max_size()) throw "Maximum container volume";
    if (this != &other) {
      iterator this_begin = begin();
      iterator this_end = end();
      iterator other_begin = other.begin();
      iterator other_end = other.end();

      while (this_begin != this_end && other_begin != other_end) {
        if (*other_begin < *this_begin) {
          node_type *tmp = other_begin.node_;
          ++other_begin;
          UnAttachNode(tmp);
          InsertNodeBefore(this_begin.node_, tmp);
          ++size_;
          --other.size_;
        } else {
          ++this_begin;
        }
      }
      splice(end(), other);
    }
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

  void sort() { QuickSort(begin(), --end(), size_); }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PART 3
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  template <typename... Args>
  iterator emplace(const_iterator pos, Args &&... args) noexcept {
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
  void emplace_back(Args &&... args) {
    emplace(end(), std::forward<Args>(args)...);
  }

  template <typename... Args>
  void emplace_front(Args &&... args) {
    emplace(begin(), std::forward<Args>(args)...);
  }

 private:
  // удаляет узел из списка, отсоединяя его от соседних элементов (merge)
  void UnAttachNode(node_type *node) noexcept {
    node->prev_->next_ = node->next_;
    node->next_->prev_ = node->prev_;
    node->next_ = node;
    node->prev_ = node;
  }

  // вставляет новый узел перед заданным элементом (merge)
  void InsertNodeBefore(node_type *pos_node, node_type *new_node) noexcept {
    new_node->prev_ = pos_node->prev_;
    new_node->next_ = pos_node;
    pos_node->prev_->next_ = new_node;
    pos_node->prev_ = new_node;
  }

  void QuickSort(iterator left, iterator right, size_type list_size) {
    if (left != right && list_size > 1) {
      iterator swap_ = left;
      iterator pivot_ = left;
      iterator l_tmp = left;
      iterator r_tmp = right;
      --swap_;
      --pivot_;
      size_type pos = 0;
      while (pos < list_size / 2) {
        ++pivot_;
        ++pos;
      }
      value_type pivot = *pivot_;
      pos = 0;
      pivot_.node_->SwapValues(right.node_);
      while (l_tmp != r_tmp) {
        if (*l_tmp < pivot) {
          ++swap_;
          ++pos;
          l_tmp.node_->SwapValues(swap_.node_);
          ++l_tmp;
        } else if (*l_tmp == pivot) {
          --r_tmp;
          l_tmp.node_->SwapValues(r_tmp.node_);
        } else {
          ++l_tmp;
        }
      }
      iterator next_step_left = swap_;
      size_type next_step_left_size = pos;
      size_type next_step_right_size = list_size - pos - 1;
      ++swap_;
      while (r_tmp != right) {
        swap_.node_->SwapValues(r_tmp.node_);
        ++swap_;
        ++r_tmp;
        --next_step_right_size;
      }
      swap_.node_->SwapValues(right.node_);
      ++swap_;
      iterator next_step_right = swap_;
      QuickSort(left, next_step_left, next_step_left_size);
      QuickSort(next_step_right, right, next_step_right_size);
    }
  }

  node_type *head_;
  size_type size_;
};

}  // namespace s21

#endif  // SRC_S21_LIST_H
