#ifndef RB_MAP_TREE_H
#define RB_MAP_TREE_H

#include <concepts>
#include <type_traits>

template <std::totally_ordered KeyType, class ValueType> class RBTree
{
  private:
    using value_type = std::pair<KeyType, ValueType>;

    struct RBTreeNode
    {
        value_type node_value;
        const KeyType &key = node_value.first;
        RBTreeNode *parent = nullptr;
        enum class Color : bool
        {
            RED,
            BLACK
        } color = Color::BLACK;
        std::unique_ptr<RBTreeNode> left_child;
        std::unique_ptr<RBTreeNode> right_child;
        RBTreeNode() = default;
        RBTreeNode(const KeyType &key, const ValueType &value)
        {
            node_value.first = key;
            node_value.second = value;
        }
        RBTreeNode(KeyType &&key, ValueType &&value)
        {
            node_value.first = std::move(key);
            node_value.second = std::move(value);
        }
        RBTreeNode(const KeyType &key, ValueType &&value)
        {
            node_value.first = key;
            node_value.second = std::move(value);
        }
        RBTreeNode(KeyType &&key, const ValueType &value)
        {
            node_value.first = std::move(key);
            node_value.second = value;
        }
        bool IsLeftChild()
        {
            return this == parent->left_child.get();
        }
    };

  public:
    class TreeIterator
    {
      public:
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;

      public:
        TreeIterator() = delete;
        explicit TreeIterator(RBTreeNode *ptr) : node_ptr_(ptr)
        {
        }
        TreeIterator(const TreeIterator &other) = default;
        TreeIterator &operator=(const TreeIterator &other) = default;

        [[nodiscard]] bool operator==(const TreeIterator &other) const
        {
            return node_ptr_ == other.node_ptr_;
        }
        [[nodiscard]] bool operator!=(const TreeIterator &other) const
        {
            return node_ptr_ != other.node_ptr_;
        }

        reference operator*()
        {
            return node_ptr_->node_value;
        }
        const_reference operator*() const
        {
            return node_ptr_->node_value;
        }
        pointer operator->()
        {
            return &(node_ptr_->node_value);
        }
        const_pointer operator->() const
        {
            return &(node_ptr_->node_value);
        }

        TreeIterator &operator++()
        {
            this->node_ptr_ = RBTree::next(node_ptr_);
            return (*this);
        }
        TreeIterator &operator--()
        {
            this->node_ptr_ = RBTree::previous(node_ptr_);
            return (*this);
        }
        TreeIterator operator++(int)
        {
            auto temp(*this);
            this->node_ptr_ = RBTree::next(node_ptr_);
            return temp;
        }
        TreeIterator operator--(int)
        {
            auto temp(*this);
            this->node_ptr_ = RBTree::previous(node_ptr_);
            return temp;
        }

        RBTreeNode *GetUnderlyingNodePtr()
        {
            return node_ptr_;
        }

      private:
        RBTreeNode *node_ptr_{};
    };

    using iterator = TreeIterator;

  public:
    [[nodiscard]] std::size_t Size() const
    {
        return size_;
    }

    std::pair<iterator, bool> Insert(const value_type &element)
    {
        const auto parent = getParent(element.first);
        if (parent == nullptr)
        {
            return {iterator(parent), false};
        }

        return insertInternal(parent, getNewNode(parent, element));
    }

    std::pair<iterator, bool> Insert(value_type &&element)
    {
        const auto [parent, result] = getParent(element.first);
        if (result == false)
        {
            return {iterator(parent), false};
        }

        return insertInternal(parent, getNewNode(parent, std::move(element)));
    }

    template <typename First, typename Second> std::pair<iterator, bool> Emplace(First &&first, Second &&second)
    {
        static_assert(std::is_same_v<typename std::remove_const_t<std::remove_reference_t<First>>, KeyType>);
        static_assert(std::is_same_v<typename std::remove_const_t<std::remove_reference_t<Second>>, ValueType>);

        const auto [parent, result] = getParent(first);
        if (result == false)
        {
            return {iterator(parent), false};
        }

        return insertInternal(parent, getNewNode(parent, std::forward<First>(first), std::forward<Second>(second)));
    }

    iterator Erase(iterator to_delete)
    {
        const RBTreeNode *node_to_delete = to_delete.GetUnderlyingNodePtr(); // Node to delete
        if (node_to_delete->left_child == nullptr)
        {
            // Node being deleted has no left child
        }
    }

    static iterator begin(RBTree &tree)
    {
        return iterator(tree.min_node_ptr_);
    }

    static iterator end(RBTree &tree)
    {
        return iterator(&(tree.end_node_));
    }

    iterator begin()
    {
        return iterator(min_node_ptr_);
    }

    iterator end()
    {
        return iterator(&end_node_);
    }

  private:
    [[nodiscard]] static RBTreeNode *leftMost(RBTreeNode *node)
    {
        while (node->left_child)
        {
            node = node->left_child.get();
        }
        return node;
    }

    [[nodiscard]] static RBTreeNode *rightMost(RBTreeNode *node)
    {
        while (node->right_child)
        {
            node = node->right_child.get();
        }
        return node;
    }

    [[nodiscard]] static RBTreeNode *next(const RBTreeNode *node)
    {
        if (node->right_child)
        {
            return leftMost(node->right_child.get());
        }
        auto current = node;
        while (current->parent && current->parent->left_child.get() != current)
        {
            current = current->parent;
        }
        return current->parent;
    }

    [[nodiscard]] static RBTreeNode *previous(const RBTreeNode *node)
    {
        if (node->left_child)
        {
            return rightMost(node->left_child.get());
        }
        auto current = node;
        while (current->parent && current->parent->left_child.get() == current)
        {
            current = current->parent;
        }
        return current->parent;
    }

    static void leftRotate(RBTreeNode *x)
    {
        assert(x->right_child != nullptr);
        RBTreeNode *grandparent = x->parent;
        RBTreeNode *y = x->right_child.get();
        std::unique_ptr<RBTreeNode> temp;
        std::unique_ptr<RBTreeNode> temp2;
        if (x->parent->left_child.get() == x)
        {
            temp.swap(grandparent->left_child);
            temp2.swap(y->left_child);
            temp2.swap(x->right_child);
            temp.swap(y->left_child);
            temp2.swap(grandparent->left_child);
            assert(temp == nullptr);
            assert(temp2 == nullptr);
        }
        else
        {
            temp.swap(grandparent->right_child);
            temp2.swap(y->left_child);
            temp2.swap(x->right_child);
            temp.swap(y->left_child);
            temp2.swap(grandparent->right_child);
            assert(temp == nullptr);
            assert(temp2 == nullptr);
        }
    }

    static void rightRotate(RBTreeNode *x)
    {
        assert(x->left_child != nullptr);
        RBTreeNode *grandparent = x->parent;
        RBTreeNode *y = x->left_child.get();
        std::unique_ptr<RBTreeNode> temp;
        std::unique_ptr<RBTreeNode> temp2;
        if (x->parent->left_child.get() == x)
        {
            temp.swap(grandparent->left_child);
            temp2.swap(y->right_child);
            temp2.swap(x->left_child);
            temp.swap(y->right_child);
            temp2.swap(grandparent->left_child);
            assert(temp == nullptr);
            assert(temp2 == nullptr);
        }
        else
        {
            temp.swap(grandparent->right_child);
            temp2.swap(y->right_child);
            temp2.swap(x->left_child);
            temp.swap(y->right_child);
            temp2.swap(grandparent->right_child);
            assert(temp == nullptr);
            assert(temp2 == nullptr);
        }
    }

    void insertFixup(RBTreeNode *z)
    {
        using Color = typename RBTreeNode::Color;
        while (z->parent->color == RBTreeNode::Color::RED)
        {
            if (z->parent == z->parent->parent->left_child.get())
            {
                // Parent is a left child
                RBTreeNode *aunt = z->parent->parent->right_child.get();
                if (aunt && aunt->color == Color::RED)
                {
                    z->parent->color = Color::BLACK;
                    aunt->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else
                {
                    // Aunt is black
                    if (z == z->parent->right_child.get())
                    {
                        // z is a right child
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                // Parent is a right child
                RBTreeNode *aunt = z->parent->parent->left_child.get();
                if (aunt && aunt->color == Color::RED)
                {
                    z->parent->color = Color::BLACK;
                    aunt->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else
                {
                    // Aunt is black
                    if (z == z->parent->left_child.get())
                    {
                        // z is a left child
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        // Root is force to become BLACK to maintain RBTree property invariance
        end_node_.left_child->color = Color::BLACK;
    }

    // Returns the {parent, true} of the where "parent" is the future parent of the key that's about to be added, but if
    // the key already exists then the function returns {node, false}  where node->key == key
    [[nodiscard]] std::pair<RBTreeNode *, bool> getParent(const KeyType &key)
    {
        RBTreeNode *current = end_node_.left_child.get();
        RBTreeNode *previous = &end_node_;
        while (current)
        {
            if (current->key == key)
            {
                // Duplicate key, the insert failed
                return {current, false};
            }
            else if (current->key > key)
            {
                previous = current;
                current = current->left_child.get();
            }
            else
            {
                previous = current;
                current = current->right_child.get();
            }
        }
        return {previous, true};
    }

    [[nodiscard]] std::pair<iterator, bool> insertInternal(RBTreeNode *parent, std::unique_ptr<RBTreeNode> &&new_node)
    {
        const auto new_node_raw_ptr = new_node.get();

        if (min_node_ptr_ == nullptr || min_node_ptr_->key > new_node_raw_ptr->key)
        {
            min_node_ptr_ = new_node_raw_ptr;
        }

        if (parent == &end_node_)
        {
            end_node_.left_child = std::move(new_node);
        }
        else if (parent->key > new_node->key)
        {
            parent->left_child = std::move(new_node);
        }
        else
        {
            parent->right_child = std::move(new_node);
        }

        insertFixup(new_node_raw_ptr);
        ++size_;

        return {iterator(new_node_raw_ptr), true};
    }

    [[nodiscard]] std::unique_ptr<RBTreeNode> getNewNode(RBTreeNode *parent, value_type &&element) const
    {
        auto new_node = std::make_unique<RBTreeNode>();
        new_node->parent = parent;
        new_node->node_value = std::move(element);
        return new_node;
    }

    [[nodiscard]] std::unique_ptr<RBTreeNode> getNewNode(RBTreeNode *parent, const value_type &element) const
    {
        auto new_node = std::make_unique<RBTreeNode>();
        new_node->parent = parent;
        new_node->node_value = element;
        return new_node;
    }

    template <typename First, typename Second>
    [[nodiscard]] std::unique_ptr<RBTreeNode> getNewNode(RBTreeNode *parent, First &&first, Second &&second) const
    {
        auto new_node = std::make_unique<RBTreeNode>(std::forward<First>(first), std::forward<Second>(second));
        new_node->parent = parent;
        return new_node;
    }

  private:
    // "end_node_" will always have its left_child pointing to the root of the
    // tree
    RBTreeNode end_node_;
    RBTreeNode *min_node_ptr_ = nullptr;
    size_t size_ = 0;
};

#endif // RB_MAP_TREE_H
