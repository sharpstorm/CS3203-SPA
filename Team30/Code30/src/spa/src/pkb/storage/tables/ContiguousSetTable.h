#pragma once

#include <set>
#include <cassert>
#include "IBaseSetTable.h"
#include "ContiguousTable.h"

using std::set;

template<typename V>
class ContiguousSetTable : public IBaseSetTable<int, V>,
                           public ContiguousTable<set<V>> {
 public:
  explicit ContiguousSetTable(int size = 1) : ContiguousTable<set<V>>(
      size) {}

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    this->resizeIfExceed(key);
    this->table[key].insert(value);
  }

  const set<V>& get(int key) const override {
    assert(key != 0);

    return ContiguousTable<set<V>>::get(key);
  }
};
