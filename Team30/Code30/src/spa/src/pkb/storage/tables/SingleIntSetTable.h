#pragma once

#include <memory>
#include <unordered_set>

#include "IBaseSetTable.h"
#include "IntTable.h"
#include "pkb/storage/iterators/ItemIterator.h"

using std::unordered_set, std::make_unique;

template <typename V>
class SingleIntSetTable : public IBaseSetTable<int, V>, public IntTable<V> {
 public:
  using IntTable<V>::IntTable;

  void insert(int key, V value) override {
    return IntTable<V>::insert(key, value);
  }

  int size() const override { return IntTable<V>::size(); };

  bool isEmpty() const override { return IntTable<V>::isEmpty(); }

  bool contains(int key, V value) const override {
    return this->get(key) == value;
  };

  bool containsKey(int key) const override { return this->get(key) != V(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(int key) override {
    return make_unique<ItemIterator<V>>(this->get(key));
  }
};
