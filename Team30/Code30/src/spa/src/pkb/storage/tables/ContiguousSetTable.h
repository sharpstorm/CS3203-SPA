#pragma once

#include <cassert>
#include <set>

#include "ContiguousTable.h"
#include "IBaseSetTable.h"

using std::set;

template <typename V>
class ContiguousSetTable : public IBaseSetTable<int, V>,
                           public ContiguousTable<set<V>> {
 public:
  explicit ContiguousSetTable(int size = 1) : ContiguousTable<set<V>>(size) {}

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    this->resizeIfExceed(key);
    this->table[key].insert(value);
  }

  const set<V>& get(int key) const override {
    return ContiguousTable<set<V>>::get(key);
  }

  void forEach(pkb::Function<int, set<V>> function,
               pkb::Terminator<int, set<V>> terminator) const override {
    return ContiguousTable<set<V>>::forEach(function, terminator);
  }

  int size() const override {
    return ContiguousTable<set<V>>::size();
  };
};
