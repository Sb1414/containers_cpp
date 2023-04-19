#ifndef CONTAINERS_S21_TREE_H_
#define CONTAINERS_S21_TREE_H_

#include <functional>
#include <limits>
#include <vector>

namespace s21 {
enum color { black, red };

// Параметр Key указывает тип данных, которые будут храниться в дереве
// Параметр Comparison опциональный и в качестве параметра шаблона можно передать компаратор с типом, который должен быть сравнимым с типом Key
template <typename Key, typename Comparison = std::less<Key>>
class tree {
 private:
  struct Node;
  struct Iterator;
  struct IteratorConst;

 public:
  using key_type = Key;
  using reference = key_type &;
  using const_reference = const key_type &;
  using iterator = Iterator;
  using const_iterator = IteratorConst;
  using size_type = std::size_t;

  tree() : head_(new Node), size_(0U) {}

  tree(const tree &other) : tree() {
    if (other.Size() > 0) {
      CopyTreeFromOther(other);
    }
  }

  tree(tree &&other) noexcept : tree() { Swap(other); }

  tree &operator=(const tree &other) {
    if (this != &other) {
      if (other.Size() > 0) {
        CopyTreeFromOther(other);
      } else {
        Clear();
      }
    }
    return *this;
  }

  tree &operator=(tree &&other) noexcept {
    Clear();
    Swap(other);
    return *this;
  }

  ~tree() {
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
    return ((std::numeric_limits<size_type>::max() / 2) - sizeof(tree) -
            sizeof(Node)) /
           sizeof(Node);
  }

  iterator Begin() noexcept { return iterator(MostLeft()); }

  iterator End() noexcept { return iterator(head_); }

  void Merge(tree &other) {
    if (this != &other) {
      iterator other_begin = other.Begin();
      while (other.size_ > 0) {
        Node *moving_node = other_begin.node_;
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
        moving_node->left_ = nullptr;
        moving_node->right_ = nullptr;
        moving_node->parent_ = nullptr;
        moving_node->color_ = red;
        --other.size_;
        Insert(Root(), moving_node, false);
      }
      other.InitializeHead();
    }
  }

  void MergeUnique(tree &other) {
    if (this != &other) {
      iterator other_begin = other.Begin();
      iterator other_end = other.End();

      while (other_begin != other_end) {
        iterator result_it = Find(other_begin.node_->key_);
        if (result_it == End()) {
          iterator tmp = other_begin;
          ++other_begin;
          Node *moving_node = other.ExtractNode(tmp);
          Insert(Root(), moving_node, false);
        } else {
          ++other_begin;
        }
      }
    }
  }

  iterator Insert(const key_type &key) {
    Node *new_node = new Node{key};
    return Insert(Root(), new_node, false).first;
  }

  std::pair<iterator, bool> InsertUnique(const key_type &key) {
    Node *new_node = new Node{key};
    std::pair<iterator, bool> result = Insert(Root(), new_node, true);
    if (result.second == false) {
      delete new_node;
    }
    return result;
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> Emplace(Args &&...args) {
    std::vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));
    for (auto item : {std::forward<Args>(args)...}) {
      Node *new_node = new Node(std::move(item));
      std::pair<iterator, bool> result_insert = Insert(Root(), new_node, false);
      result.push_back(result_insert);
    }
    return result;
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> EmplaceUnique(Args &&...args) {
    std::vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));

    for (auto item : {std::forward<Args>(args)...}) {
      Node *new_node = new Node(std::move(item));
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
    Node *start = Root();
    Node *result = End().node_;
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
    Node *start = Root();
    Node *result = End().node_;

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
    Node *result = ExtractNode(pos);
    delete result;
  }

  void Swap(tree &other) noexcept {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
    std::swap(cmp_, other.cmp_);
  }

 private:
  void CopyTreeFromOther(const tree &other) {
    Node *other_copy_root = CopyTree(other.Root(), nullptr);
    Clear();
    Root() = other_copy_root;
    Root()->parent_ = head_;
    MostLeft() = SearchMinimum(Root());
    MostRight() = SearchMaximum(Root());
    size_ = other.size_;
    cmp_ = other.cmp_;
  }

  Node *CopyTree(const Node *node, Node *parent) {
    Node *copy = new Node{node->key_, node->color_};
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

  void Destroy(Node *node) noexcept {
    if (node == nullptr) return;
    Destroy(node->left_);
    Destroy(node->right_);
    delete node;
  }

  void InitializeHead() {
    Root() = nullptr;
    MostLeft() = head_;
    MostRight() = head_;
  }

  Node *&Root() { return head_->parent_; }

  const Node *Root() const { return head_->parent_; }

  Node *&MostLeft() { return head_->left_; }

  Node *&MostRight() { return head_->right_; }

  std::pair<iterator, bool> Insert(Node *root, Node *new_node,
                                   bool unique_only) {
    Node *node = root;
    Node *parent = nullptr;
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
      new_node->color_ = black;
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

  void BalancingInsert(Node *node) {
    Node *parent = node->parent_;
    while (node != Root() && parent->color_ == red) {
      Node *gparent = parent->parent_;
      if (gparent->left_ == parent) {
        Node *uncle = gparent->right_;
        if (uncle != nullptr && uncle->color_ == red) {
          parent->color_ = black;
          uncle->color_ = black;
          gparent->color_ = red;
          node = gparent;
          parent = node->parent_;
        } else {
          if (parent->right_ == node) {
            RotateLeft(parent);
            std::swap(parent, node);
          }
          RotateRight(gparent);
          gparent->color_ = red;
          parent->color_ = black;
          break;
        }
      } else {
        Node *uncle = gparent->left_;
        if (uncle != nullptr && uncle->color_ == red) {
          parent->color_ = black;
          uncle->color_ = black;
          gparent->color_ = red;
          node = gparent;
          parent = node->parent_;
        } else {
          if (parent->left_ == node) {
            RotateRight(parent);
            std::swap(parent, node);
          }
          RotateLeft(gparent);
          gparent->color_ = red;
          parent->color_ = black;
          break;
        }
      }
    }
    Root()->color_ = black;
  }

  void RotateRight(Node *node) {
    Node *const pivot = node->left_;
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

  void RotateLeft(Node *node) {
    Node *const pivot = node->right_;
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

  Node *ExtractNode(iterator pos) {
    if (pos == End()) {
      return nullptr;
    }

    Node *deleted_node = pos.node_;
    if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {
      Node *replace = SearchMinimum(deleted_node->right_);
      SwapNodesForErase(deleted_node, replace);
    }
    if (deleted_node->color_ == black &&
        ((deleted_node->left_ == nullptr && deleted_node->right_ != nullptr) ||
         (deleted_node->left_ != nullptr && deleted_node->right_ == nullptr))) {
      Node *replace;
      if (deleted_node->left_ != nullptr) {
        replace = deleted_node->left_;
      } else {
        replace = deleted_node->right_;
      }
      SwapNodesForErase(deleted_node, replace);
    }
    if (deleted_node->color_ == black && deleted_node->left_ == nullptr &&
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
    deleted_node->left_ = nullptr;
    deleted_node->right_ = nullptr;
    deleted_node->parent_ = nullptr;
    deleted_node->color_ = red;
    return deleted_node;
  }

  void SwapNodesForErase(Node *node, Node *other) {
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

  void EraseBalancing(Node *deleted_node) {
    Node *check_node = deleted_node;
    Node *parent = deleted_node->parent_;
    while (check_node != Root() && check_node->color_ == black) {
      if (check_node == parent->left_) {
        Node *sibling = parent->right_;
        if (sibling->color_ == red) {
          std::swap(sibling->color_, parent->color_);
          RotateLeft(parent);
          parent = check_node->parent_;
          sibling = parent->right_;
        }
        if (sibling->color_ == black &&
            (sibling->left_ == nullptr || sibling->left_->color_ == black) &&
            (sibling->right_ == nullptr || sibling->right_->color_ == black)) {
          sibling->color_ = red;
          if (parent->color_ == red) {
            parent->color_ = black;
            break;
          }
          check_node = parent;
          parent = check_node->parent_;
        } else {
          if (sibling->left_ != nullptr && sibling->left_->color_ == red &&
              (sibling->right_ == nullptr ||
               sibling->right_->color_ == black)) {
            std::swap(sibling->color_, sibling->left_->color_);
            RotateRight(sibling);
            sibling = parent->right_;
          }
          sibling->right_->color_ = black;
          sibling->color_ = parent->color_;
          parent->color_ = black;
          RotateLeft(parent);
          break;
        }
      } else {
        Node *sibling = parent->left_;
        if (sibling->color_ == red) {
          std::swap(sibling->color_, parent->color_);
          RotateRight(parent);
          parent = check_node->parent_;
          sibling = parent->left_;
        }
        if (sibling->color_ == black &&
            (sibling->left_ == nullptr || sibling->left_->color_ == black) &&
            (sibling->right_ == nullptr || sibling->right_->color_ == black)) {
          sibling->color_ = red;
          if (parent->color_ == red) {
            parent->color_ = black;
            break;
          }
          check_node = parent;
          parent = check_node->parent_;
        } else {
          if (sibling->right_ != nullptr && sibling->right_->color_ == red &&
              (sibling->left_ == nullptr || sibling->left_->color_ == black)) {
            std::swap(sibling->color_, sibling->right_->color_);
            RotateLeft(sibling);
            sibling = parent->left_;
          }
          sibling->left_->color_ = black;
          sibling->color_ = parent->color_;
          parent->color_ = black;
          RotateRight(parent);
          break;
        }
      }
    }
  }

  Node *SearchMinimum(Node *node) const {
    while (node->left_ != nullptr) {
      node = node->left_;
    };
    return node;
  }

  Node *SearchMaximum(Node *node) const {
    while (node->right_ != nullptr) {
      node = node->right_;
    };
    return node;
  }

  struct Node {
    Node()
        : parent_(nullptr),
          left_(this),
          right_(this),
          key_(key_type{}),
          color_(red) {}

    Node(const key_type &key)
        : parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          key_(key),
          color_(red) {}

    Node(key_type &&key)
        : parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          key_(std::move(key)),
          color_(red) {}

    Node(key_type key, color color_)
        : parent_(nullptr),
          left_(nullptr),
          right_(nullptr),
          key_(key),
          color_(color_) {}

    Node *Next() const noexcept {
      Node *node = const_cast<Node *>(this);
      if (node->color_ == red &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->left_;
      } else if (node->right_ != nullptr) {
        node = node->right_;
        while (node->left_ != nullptr) {
          node = node->left_;
        }
      } else {
        Node *parent = node->parent_;
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

    Node *parent_;
    Node *left_;
    Node *right_;
    key_type key_;
    color color_;
  };

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = tree::key_type;
    using pointer = value_type *;
    using reference = value_type &;

    explicit Iterator(Node *node) : node_(node) {}

    reference operator*() const noexcept { return node_->key_; }

    iterator &operator++() noexcept {
      node_ = node_->Next();
      return *this;
    }

    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const iterator &other) const noexcept {
      return node_ != other.node_;
    }

    Node *node_;
  };

  struct IteratorConst {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = tree::key_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    IteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->key_; }

    const_iterator &operator++() noexcept {
      node_ = node_->Next();
      return *this;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ != it2.node_;
    }

    const Node *node_;
  };

  Node *head_;
  size_type size_;
  Comparison cmp_;
};

}  // namespace s21

#endif  // CONTAINERS_S21_TREE_H_
