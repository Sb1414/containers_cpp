#ifndef S21_CONTAINERS_S21_CONTAINERS_S21_TREE_H_
#define S21_CONTAINERS_S21_CONTAINERS_S21_TREE_H_

#include <functional>
#include <limits>
#include <vector>

namespace s21 {
enum RedBlackTreeColor { kBlack, kRed };

template <typename Key, typename Comparator = std::less<Key>>
class RedBlackTree {
 private:
  struct RedBlackTreeNode;
  struct RedBlackTreeIterator;
  struct RedBlackTreeIteratorConst;
#if defined(S21_CONTAINERS_TREE_TEST_HELPER)
  template <typename U>
  friend class RedBlackTreeTestHelper;
#endif
 public:
  using key_type = Key;
  using reference = key_type &;
  using const_reference = const key_type &;
  using iterator = RedBlackTreeIterator;
  using const_iterator = RedBlackTreeIteratorConst;
  using size_type = std::size_t;
  using tree_type = RedBlackTree;
  using tree_node = RedBlackTreeNode;
  using tree_color = RedBlackTreeColor;

  RedBlackTree() : head_(new tree_node), size_(0U) {}

  RedBlackTree(const tree_type &other) : RedBlackTree() {
    if (other.Size() > 0) {
      CopyTreeFromOther(other);
    }
  }

  RedBlackTree(tree_type &&other) noexcept : RedBlackTree() { Swap(other); }

  tree_type &operator=(const tree_type &other) {
    if (this != &other) {
      if (other.Size() > 0) {
        CopyTreeFromOther(other);
      } else {
        Clear();
      }
    }
    return *this;
  }

  tree_type &operator=(tree_type &&other) noexcept {
    Clear();
    Swap(other);
    return *this;
  }

  ~RedBlackTree() {
    Clear();
    delete head_;
    head_ = nullptr;
  }

  void Clear() noexcept {
    Destroy(Root());
    InitializeHead();
    size_ = 0;
  }

  size_type Size() const noexcept { return size_; }

  bool Empty() const noexcept { return size_ == 0; }

  size_type MaxSize() const noexcept {
    return ((std::numeric_limits<size_type>::max() / 2) - sizeof(tree_type) -
            sizeof(tree_node)) /
           sizeof(tree_node);
  }

  iterator Begin() noexcept { return iterator(MostLeft()); }

  const_iterator Begin() const noexcept { return const_iterator(MostLeft()); }

  iterator End() noexcept { return iterator(head_); }

  const_iterator End() const noexcept { return const_iterator(head_); }

  void Merge(tree_type &other) {
    if (this != &other) {
      iterator other_begin = other.Begin();
      while (other.size_ > 0) {
        tree_node *moving_node = other_begin.node_;
        ++other_begin;
        if (moving_node->left_ != nullptr) {
          moving_node->left_->parent_ = moving_node->parent_;
        }
        if (moving_node->right_ != nullptr) {
          moving_node->right_->parent_ = moving_node->parent_;
        }
        if (moving_node->parent_->left_ == moving_node) {
          moving_node->parent_->left_ = nullptr;
        }
        if (moving_node->parent_->right_ == moving_node) {
          moving_node->parent_->right_ = nullptr;
        }
        moving_node->ToDefault();
        --other.size_;
        Insert(Root(), moving_node, false);
      }
      other.InitializeHead();
    }
  }

  void MergeUnique(tree_type &other) {
    if (this != &other) {
      iterator other_begin = other.Begin();
      iterator other_end = other.End();

      while (other_begin != other_end) {
        iterator result_it = Find(other_begin.node_->key_);
        if (result_it == End()) {
          iterator tmp = other_begin;
          ++other_begin;
          tree_node *moving_node = other.ExtractNode(tmp);
          Insert(Root(), moving_node, false);
        } else {
          ++other_begin;
        }
      }
    }
  }

  iterator Insert(const key_type &key) {
    tree_node *new_node = new tree_node{key};
    return Insert(Root(), new_node, false).first;
  }

  std::pair<iterator, bool> InsertUnique(const key_type &key) {
    tree_node *new_node = new tree_node{key};
    std::pair<iterator, bool> result = Insert(Root(), new_node, true);
    if (result.second == false) {
      delete new_node;
    }

    return result;
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> Emplace(Args &&... args) {
    std::vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));
    for (auto item : {std::forward<Args>(args)...}) {
      tree_node *new_node = new tree_node(std::move(item));
      std::pair<iterator, bool> result_insert = Insert(Root(), new_node, false);
      result.push_back(result_insert);
    }
    return result;
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> EmplaceUnique(Args &&... args) {
    std::vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));

    for (auto item : {std::forward<Args>(args)...}) {
      tree_node *new_node = new tree_node(std::move(item));
      std::pair<iterator, bool> result_insert = Insert(Root(), new_node, true);
      if (result_insert.second == false) {
        delete new_node;
      }
      result.push_back(result_insert);
    }
    return result;
  }

  iterator Find(const_reference key) {
    iterator result = LowerBound(key);
    if (result == End() || cmp_(key, *result)) {
      return End();
    }
    return result;
  }

  iterator LowerBound(const_reference key) {
    tree_node *start = Root();
    tree_node *result = End().node_;
    while (start != nullptr) {
      if (!cmp_(start->key_, key)) {
        result = start;
        start = start->left_;
      } else {
        start = start->right_;
      }
    }
    return iterator(result);
  }

  iterator UpperBound(const_reference key) {
    tree_node *start = Root();
    tree_node *result = End().node_;

    while (start != nullptr) {
      if (cmp_(key, start->key_)) {
        result = start;
        start = start->left_;
      } else
        start = start->right_;
    }
    return iterator(result);
  }

  void Erase(iterator pos) noexcept {
    tree_node *result = ExtractNode(pos);
    delete result;
  }

  void Swap(tree_type &other) noexcept {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
    std::swap(cmp_, other.cmp_);
  }

  bool CheckTree() const noexcept {
    if (head_->color_ == kBlack) {
      return false;
    }

    if (Root() == nullptr) {
      return true;
    }

    if (Root()->color_ == kRed) {
      return false;
    }

    if (checkRedNodes(Root()) == false) {
      return false;
    }

    if (ComputeBlackHeight(Root()) == -1) {
      return false;
    }

    return true;
  }

 private:
  void CopyTreeFromOther(const tree_type &other) {
    tree_node *other_copy_root = CopyTree(other.Root(), nullptr);
    Clear();
    Root() = other_copy_root;
    Root()->parent_ = head_;
    MostLeft() = SearchMinimum(Root());
    MostRight() = SearchMaximum(Root());
    size_ = other.size_;
    cmp_ = other.cmp_;
  }

  [[nodiscard]] tree_node *CopyTree(const tree_node *node, tree_node *parent) {
    tree_node *copy = new tree_node{node->key_, node->color_};
    try {
      if (node->left_) {
        copy->left_ = CopyTree(node->left_, copy);
      }

      if (node->right_) {
        copy->right_ = CopyTree(node->right_, copy);
      }
    } catch (...) {
      Destroy(copy);
      throw;
    }

    copy->parent_ = parent;
    return copy;
  }

  void Destroy(tree_node *node) noexcept {
    if (node == nullptr) return;
    Destroy(node->left_);
    Destroy(node->right_);
    delete node;
  }

  void InitializeHead() noexcept {
    Root() = nullptr;
    MostLeft() = head_;
    MostRight() = head_;
  }

  tree_node *&Root() { return head_->parent_; }

  const tree_node *Root() const { return head_->parent_; }

  tree_node *&MostLeft() { return head_->left_; }

  const tree_node *MostLeft() const { return head_->left_; }

  tree_node *&MostRight() { return head_->right_; }

  std::pair<iterator, bool> Insert(tree_node *root, tree_node *new_node,
                                   bool unique_only) {
    tree_node *node = root;
    tree_node *parent = nullptr;
    while (node != nullptr) {
      parent = node;
      if (cmp_(new_node->key_, node->key_)) {
        node = node->left_;
      } else {
        if (unique_only == false) {
          node = node->right_;
        } else {
          if (cmp_(node->key_, new_node->key_)) {
            node = node->right_;
          } else {
            return {iterator(node), false};
          }
        }
      }
    }
    if (parent != nullptr) {
      new_node->parent_ = parent;
      if (cmp_(new_node->key_, parent->key_)) {
        parent->left_ = new_node;
      } else {
        parent->right_ = new_node;
      }
    } else {
      new_node->color_ = kBlack;
      new_node->parent_ = head_;
      Root() = new_node;
    }
    ++size_;
    if (MostLeft() == head_ || MostLeft()->left_ != nullptr) {
      MostLeft() = new_node;
    }
    if (MostRight() == head_ || MostRight()->right_ != nullptr) {
      MostRight() = new_node;
    }
    BalancingInsert(new_node);
    return {iterator(new_node), true};
  }

  void BalancingInsert(tree_node *node) {
    tree_node *parent = node->parent_;
    while (node != Root() && parent->color_ == kRed) {
      tree_node *gparent = parent->parent_;
      if (gparent->left_ == parent) {
        tree_node *uncle = gparent->right_;
        if (uncle != nullptr && uncle->color_ == kRed) {
          parent->color_ = kBlack;
          uncle->color_ = kBlack;
          gparent->color_ = kRed;
          node = gparent;
          parent = node->parent_;
        } else {
          if (parent->right_ == node) {
            RotateLeft(parent);
            std::swap(parent, node);
          }
          RotateRight(gparent);
          gparent->color_ = kRed;
          parent->color_ = kBlack;
          break;
        }
      } else {
        tree_node *uncle = gparent->left_;
        if (uncle != nullptr && uncle->color_ == kRed) {
          parent->color_ = kBlack;
          uncle->color_ = kBlack;
          gparent->color_ = kRed;
          node = gparent;
          parent = node->parent_;
        } else {
          if (parent->left_ == node) {
            RotateRight(parent);
            std::swap(parent, node);
          }
          RotateLeft(gparent);
          gparent->color_ = kRed;
          parent->color_ = kBlack;
          break;
        }
      }
    }
    Root()->color_ = kBlack;
  }

  void RotateRight(tree_node *node) noexcept {
    tree_node *const pivot = node->left_;
    pivot->parent_ = node->parent_;
    if (node == Root()) {
      Root() = pivot;
    } else if (node->parent_->left_ == node) {
      node->parent_->left_ = pivot;
    } else {
      node->parent_->right_ = pivot;
    }
    node->left_ = pivot->right_;
    if (pivot->right_ != nullptr) {
      pivot->right_->parent_ = node;
    }
    node->parent_ = pivot;
    pivot->right_ = node;
  }

  void RotateLeft(tree_node *node) noexcept {
    tree_node *const pivot = node->right_;
    pivot->parent_ = node->parent_;
    if (node == Root()) {
      Root() = pivot;
    } else if (node->parent_->left_ == node) {
      node->parent_->left_ = pivot;
    } else {
      node->parent_->right_ = pivot;
    }
    node->right_ = pivot->left_;
    if (pivot->left_ != nullptr) {
      pivot->left_->parent_ = node;
    }
    node->parent_ = pivot;
    pivot->left_ = node;
  }

  tree_node *ExtractNode(iterator pos) noexcept {
    if (pos == End()) {
      return nullptr;
    }

    tree_node *deleted_node = pos.node_;
    if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {
      tree_node *replace = SearchMinimum(deleted_node->right_);
      SwapNodesForErase(deleted_node, replace);
    }
    if (deleted_node->color_ == kBlack &&
        ((deleted_node->left_ == nullptr && deleted_node->right_ != nullptr) ||
         (deleted_node->left_ != nullptr && deleted_node->right_ == nullptr))) {
      tree_node *replace;
      if (deleted_node->left_ != nullptr) {
        replace = deleted_node->left_;
      } else {
        replace = deleted_node->right_;
      }
      SwapNodesForErase(deleted_node, replace);
    }
    if (deleted_node->color_ == kBlack && deleted_node->left_ == nullptr &&
        deleted_node->right_ == nullptr) {
      EraseBalancing(deleted_node);
    }
    if (deleted_node == Root()) {
      InitializeHead();
    } else {
      if (deleted_node == deleted_node->parent_->left_) {
        deleted_node->parent_->left_ = nullptr;
      } else {
        deleted_node->parent_->right_ = nullptr;
      }
      if (MostLeft() == deleted_node) {
        MostLeft() = SearchMinimum(Root());
      }
      if (MostRight() == deleted_node) {
        MostRight() = SearchMaximum(Root());
      }
    }
    --size_;
    deleted_node->ToDefault();
    return deleted_node;
  }

  void SwapNodesForErase(tree_node *node, tree_node *other) noexcept {
    if (other->parent_->left_ == other) {
      other->parent_->left_ = node;
    } else {
      other->parent_->right_ = node;
    }
    if (node == Root()) {
      Root() = other;
    } else {
      if (node->parent_->left_ == node) {
        node->parent_->left_ = other;
      } else {
        node->parent_->right_ = other;
      }
    }
    std::swap(node->parent_, other->parent_);
    std::swap(node->left_, other->left_);
    std::swap(node->right_, other->right_);
    std::swap(node->color_, other->color_);
    if (node->left_) {
      node->left_->parent_ = node;
    }
    if (node->right_) {
      node->right_->parent_ = node;
    }
    if (other->left_) {
      other->left_->parent_ = other;
    }
    if (other->right_) {
      other->right_->parent_ = other;
    }
  }

  void EraseBalancing(tree_node *deleted_node) noexcept {
    tree_node *check_node = deleted_node;
    tree_node *parent = deleted_node->parent_;
    while (check_node != Root() && check_node->color_ == kBlack) {
      if (check_node == parent->left_) {
        tree_node *sibling = parent->right_;
        if (sibling->color_ == kRed) {
          std::swap(sibling->color_, parent->color_);
          RotateLeft(parent);
          parent = check_node->parent_;
          sibling = parent->right_;
        }
        if (sibling->color_ == kBlack &&
            (sibling->left_ == nullptr || sibling->left_->color_ == kBlack) &&
            (sibling->right_ == nullptr || sibling->right_->color_ == kBlack)) {
          sibling->color_ = kRed;
          if (parent->color_ == kRed) {
            parent->color_ = kBlack;
            break;
          }
          check_node = parent;
          parent = check_node->parent_;
        } else {
          if (sibling->left_ != nullptr && sibling->left_->color_ == kRed &&
              (sibling->right_ == nullptr ||
               sibling->right_->color_ == kBlack)) {
            std::swap(sibling->color_, sibling->left_->color_);
            RotateRight(sibling);
            sibling = parent->right_;
          }
          sibling->right_->color_ = kBlack;
          sibling->color_ = parent->color_;
          parent->color_ = kBlack;
          RotateLeft(parent);
          break;
        }
      } else {
        tree_node *sibling = parent->left_;
        if (sibling->color_ == kRed) {
          std::swap(sibling->color_, parent->color_);
          RotateRight(parent);
          parent = check_node->parent_;
          sibling = parent->left_;
        }
        if (sibling->color_ == kBlack &&
            (sibling->left_ == nullptr || sibling->left_->color_ == kBlack) &&
            (sibling->right_ == nullptr || sibling->right_->color_ == kBlack)) {
          sibling->color_ = kRed;
          if (parent->color_ == kRed) {
            parent->color_ = kBlack;
            break;
          }
          check_node = parent;
          parent = check_node->parent_;
        } else {
          if (sibling->right_ != nullptr && sibling->right_->color_ == kRed &&
              (sibling->left_ == nullptr || sibling->left_->color_ == kBlack)) {
            std::swap(sibling->color_, sibling->right_->color_);
            RotateLeft(sibling);
            sibling = parent->left_;
          }
          sibling->left_->color_ = kBlack;
          sibling->color_ = parent->color_;
          parent->color_ = kBlack;
          RotateRight(parent);
          break;
        }
      }
    }
  }

  tree_node *SearchMinimum(tree_node *node) const noexcept {
    while (node->left_ != nullptr) {
      node = node->left_;
    };
    return node;
  }

  tree_node *SearchMaximum(tree_node *node) const noexcept {
    while (node->right_ != nullptr) {
      node = node->right_;
    };
    return node;
  }

  int ComputeBlackHeight(const tree_node *node) const noexcept {
    if (node == nullptr) {
      return 0;
    }
    int left_height = ComputeBlackHeight(node->left_);
    int right_height = ComputeBlackHeight(node->right_);
    int add = node->color_ == kBlack ? 1 : 0;
    if (left_height == -1 || right_height == -1 ||
        left_height != right_height) {
      return -1;
    } else {
      return left_height + add;
    }
  }

  bool checkRedNodes(const tree_node *Node) const noexcept {
    if (Node->color_ == kRed) {
      if (Node->left_ != nullptr && Node->left_->color_ == kRed) {
        return false;
      }
      if (Node->right_ != nullptr && Node->right_->color_ == kRed) {
        return false;
      }
    }
    if (Node->left_ != nullptr) {
      if (checkRedNodes(Node->left_) == false) {
        return false;
      }
    }
    if (Node->right_ != nullptr) {
      if (checkRedNodes(Node->right_) == false) {
        return false;
      }
    }
    return true;
  }

  struct RedBlackTreeNode {
    RedBlackTreeNode()
        : parent_(nullptr),
          left_(this),
          right_(this),
          key_(key_type{}),
          color_(kRed) {}

    RedBlackTreeNode(const key_type &key)
        : parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          key_(key),
          color_(kRed) {}

    RedBlackTreeNode(key_type &&key)
        : parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          key_(std::move(key)),
          color_(kRed) {}

    RedBlackTreeNode(key_type key, tree_color color)
        : parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          key_(key),
          color_(color) {}

    void ToDefault() noexcept {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      color_ = kRed;
    }

    tree_node *NextNode() const noexcept {
      tree_node *node = const_cast<tree_node *>(this);
      if (node->color_ == kRed &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->left_;
      } else if (node->right_ != nullptr) {
        node = node->right_;
        while (node->left_ != nullptr) {
          node = node->left_;
        }
      } else {
        tree_node *parent = node->parent_;
        while (node == parent->right_) {
          node = parent;
          parent = parent->parent_;
        }
        if (node->right_ != parent) {
          node = parent;
        }
      }
      return node;
    }

    tree_node *PrevNode() const noexcept {
      tree_node *node = const_cast<tree_node *>(this);
      if (node->color_ == kRed &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->right_;
      } else if (node->left_ != nullptr) {
        node = node->left_;
        while (node->right_ != nullptr) {
          node = node->right_;
        }
      } else {
        tree_node *parent = node->parent_;
        while (node == parent->left_) {
          node = parent;
          parent = parent->parent_;
        }
        if (node->left_ != parent) {
          node = parent;
        }
      }

      return node;
    }
    tree_node *parent_;
    tree_node *left_;
    tree_node *right_;
    key_type key_;
    tree_color color_;
  };

  struct RedBlackTreeIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = tree_type::key_type;
    using pointer = value_type *;
    using reference = value_type &;

    RedBlackTreeIterator() = delete;

    explicit RedBlackTreeIterator(tree_node *node) : node_(node) {}

    reference operator*() const noexcept { return node_->key_; }

    iterator &operator++() noexcept {
      node_ = node_->NextNode();
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    iterator &operator--() noexcept {
      node_ = node_->PrevNode();
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

    tree_node *node_;
  };

  struct RedBlackTreeIteratorConst {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = tree_type::key_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    RedBlackTreeIteratorConst() = delete;

    explicit RedBlackTreeIteratorConst(const tree_node *node) : node_(node) {}

    RedBlackTreeIteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->key_; }

    const_iterator &operator++() noexcept {
      node_ = node_->NextNode();
      return *this;
    }

    const_iterator operator++(int) noexcept {
      const_iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() noexcept {
      node_ = node_->PrevNode();
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

    const tree_node *node_;
  };

  tree_node *head_;
  size_type size_;
  Comparator cmp_;
};

#if defined(S21_CONTAINERS_TREE_TEST_HELPER)
#include "s21_tree_test_helper.inc"
#endif

}  // namespace s21

#endif  // S21_CONTAINERS_S21_CONTAINERS_S21_TREE_H_