template <typename Key, typename Value>
class RBTreeMap {
 public:
  RBTreeMap() {
    nil_ = new Node();
    root_ = nil_;
  }

  ~RBTreeMap() {
    clear(root_);
    delete nil_;
  }

  void insert(const Key& key, const Value& value) {
    Node* node = new Node(key, value, nil_);
    Node* parent = nil_;
    Node* current = root_;

    while (current != nil_) {
      parent = current;
      if (key < current->key) {
        current = current->left;
      } else if (key > current->key) {
        current = current->right;
      } else {
        // Key already exists, update the value
        current->value = value;
        delete node;
        return;
      }
    }

    node->parent = parent;
    if (parent == nil_) {
      root_ = node;
    } else if (key < parent->key) {
      parent->left = node;
    } else {
      parent->right = node;
    }

    insertFixup(node);
  }

  bool contains(const Key& key) const { return search(key) != nil_; }

  const Value& at(const Key& key) const {
    Node* node = search(key);
    if (node == nil_) {
      throw std::out_of_range("Key not found");
    }
    return node->value;
  }

  void clear() {
    clear(root_);
    root_ = nil_;
  }

  size_t size() const { return size(root_); }

 private:
  struct Node {
    Key key;
    Value value;
    Node* parent;
    Node* left;
    Node* right;
    int color;

    Node()
        : key(),
          value(),
          parent(nullptr),
          left(nullptr),
          right(nullptr),
          color(0) {}
    Node(const Key& key, const Value& value, Node* nil)
        : key(key),
          value(value),
          parent(nil),
          left(nil),
          right(nil),
          color(0) {}
  };

  Node* root_;
  Node* nil_;

  Node* search(const Key& key) const {
    Node* current = root_;
    while (current != nil_) {
      if (key < current->key) {
        current = current->left;
      } else if (key > current->key) {
        current = current->right;
      } else {
        return current;
      }
    }
    return nil_;
  }

  void clear(Node* node) {
    if (node == nil_) {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
  }

  size_t size(Node* node) const {
    if (node == nil_) {
      return 0;
    }
    return size(node->left) + size(node->right) + 1;
  }

  void leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nil_) {
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil_) {
      root_ = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
  }

  void rightRotate() {}
};