#ifndef RB_MAP_TREE_H
#define RB_MAP_TREE_H

template <class P>
concept Pair = requires(P p) {
  typename P::first_type;
  typename P::second_type;
  p.first;
  p.second;
  requires std::same_as<decltype(p.first), typename P::first_type>;
  requires std::same_as<decltype(p.second), typename P::second_type>;
};

// Forward declaration
template <Pair pair>
class TreeIterator;

template <Pair pair>
struct RBTreeNode {
  using KeyType = typename pair::first_type;

  pair node_value;
  const KeyType& key = node_value.first;
  RBTreeNode* parent = nullptr;
  bool is_left_child{};
  bool is_black = true;
  std::unique_ptr<RBTreeNode> left_child;
  std::unique_ptr<RBTreeNode> right_child;
};

template <class KeyType, class ValueType>
class RBTree {
 private:
  using pair = std::pair<KeyType, ValueType>;
  using NodeType = RBTreeNode<pair>;

 public:
  using iterator = TreeIterator<pair>;

  [[nodiscard]] std::size_t Size() const { return size_; }

  bool Insert(const pair& element) {
    NodeType* current = end_node_.left_child.get();
    NodeType* previous = &end_node_;
    while (current) {
      if (current->key == element.first) {
        // Duplicate key, the insert failed
        return false;
      } else if (current->key > element.first) {
        previous = current;
        current = current->left_child.get();
      } else {
        previous = current;
        current = current->right_child.get();
      }
    }

    auto new_node = std::make_unique<NodeType>();
    new_node->parent = previous;
    new_node->node_value = element;
    new_node->is_black = false;

    const auto new_node_raw_ptr = new_node.get();

    if (previous == &end_node_) {
      new_node->is_left_child = true;
      end_node_.left_child = std::move(new_node);

    } else if (previous->key > element.first) {
      new_node->is_left_child = true;
      previous->left_child = std::move(new_node);
    } else {
      new_node->is_left_child = false;
      previous->right_child = std::move(new_node);
    }

    insertFixup(new_node_raw_ptr);
    ++size_;

    return true;
  }

  [[nodiscard]] static NodeType* LeftMost(NodeType* node) {
    while (node->left_child) {
      node = node->left_child.get();
    }
    return node;
  }

  [[nodiscard]] static NodeType* RightMost(NodeType* node) {
    while (node->right_child) {
      node = node->right_child.get();
    }
    return node;
  }

  [[nodiscard]] static NodeType* Next(const NodeType* node) {
    if (node->right_child) {
      return LeftMost(node->right_child.get());
    }
    auto current = node;
    while (!current->is_left_child) {
      current = current->parent;
    }
    return current->parent;
  }

  [[nodiscard]] static NodeType* Previous(const NodeType* node) {
    if (node->left_child) {
      return RightMost(node->left_child.get());
    }
    auto current = node;
    while (current->is_left_child) {
      current = current->parent;
    }
    return current->parent;
  }

  static void LeftRotate(const NodeType* node) { assert(true); }

  iterator begin() {
    NodeType* current = end_node_.left_child.get();
    NodeType* previous = nullptr;
    while (current) {
      previous = current;
      current = Previous(current);
    }
    return iterator(previous);
  }

  iterator end() { return iterator(&end_node_); }

 private:
  void insertFixup(NodeType* inserted_node) { (void)inserted_node; }

  // "end_node_" will always have its left_child pointing to the root of the
  // tree
  NodeType end_node_;
  size_t size_ = 0;
};

template <Pair pair>
class TreeIterator {
 public:
  using pointer = pair*;
  using const_pointer = const pair*;
  using reference = pair&;
  using const_reference = const pair&;
  using NodeType = RBTreeNode<pair>;
  using KeyType = typename pair::first_type;
  using ValueType = typename pair::second_type;
  using TreeType = RBTree<KeyType, ValueType>;

 public:
  TreeIterator() = delete;
  explicit TreeIterator(NodeType* ptr) : node_ptr_(ptr) {}
  TreeIterator(const TreeIterator& other) = default;
  TreeIterator& operator=(const TreeIterator& other) = default;

  [[nodiscard]] bool operator==(const TreeIterator& other) const {
    return node_ptr_ == other.node_ptr_;
  }
  [[nodiscard]] bool operator!=(const TreeIterator& other) const {
    return node_ptr_ != other.node_ptr_;
  }

  reference operator*() { return node_ptr_->node_value; }
  const_reference operator*() const { return node_ptr_->node_value; }
  pointer operator->() { return &(node_ptr_->node_value); }
  const_pointer operator->() const { return &(node_ptr_->node_value); }

  TreeIterator& operator++() {
    this->node_ptr_ = TreeType::Next(node_ptr_);
    return (*this);
  }
  TreeIterator& operator--() {
    this->node_ptr_ = TreeType::Previous(node_ptr_);
    return (*this);
  }
  TreeIterator operator++(int) {
    auto temp(*this);
    this->node_ptr_ = NodeType::Next(node_ptr_);
    return temp;
  }
  TreeIterator operator--(int) {
    auto temp(*this);
    this->node_ptr_ = TreeType::Previous(node_ptr_);
    return temp;
  }

 private:
  RBTreeNode<pair>* node_ptr_{};
};

#endif  // RB_MAP_TREE_H
