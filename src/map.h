#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <cstddef>
#include <utility>

#include "rbtree.h"

template <class KeyType, class ValueType> class Map
{
  public:
    [[nodiscard]] std::size_t Size() const
    {
        return rb_tree_.Size();
    }
    [[nodiscard]] bool Empty() const
    {
        return Size() == 0;
    }

  private:
    RBTree<KeyType, ValueType> rb_tree_;
};

#endif // MAP_MAP_H
