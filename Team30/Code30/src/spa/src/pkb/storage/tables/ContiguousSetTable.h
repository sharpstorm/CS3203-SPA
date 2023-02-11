#pragma once

#include <unordered_set>
#include "BaseTable.h"
#include "ContiguousTable.h"

using std::unordered_set;

template<typename V>
class ContiguousSetTable : public BaseTable<int, V>,
                           public ContiguousTable<unordered_set<V>> {
 public:
  explicit ContiguousSetTable(int size = 1) : ContiguousTable<unordered_set<V>>(
      size) {}

  void set(int key, V value) override {
    this->resizeIfExceed(key);
    this->table[key].insert(value);
  }

  unordered_set<V> get(int key) const override {
    return ContiguousTable<unordered_set<V>>::get(key);
  }
};
