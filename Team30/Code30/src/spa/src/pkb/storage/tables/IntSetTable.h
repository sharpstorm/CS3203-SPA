#pragma once

#include <cassert>
#include <set>

#include "IBaseSetTable.h"
#include "IntTable.h"

using std::set;

template <typename V>
class IntSetTable : public IBaseSetTable<int, V>, public IntTable<set<V>> {
 public:
  using IntTable<set<V>>::IntTable;

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    this->table[key].insert(value);
  }

  const set<V>& get(int key) const override {
    return IntTable<set<V>>::get(key);
  }

  void forEach(pkb::Function<int, set<V>> function,
               pkb::Terminator<int, set<V>> terminator) const override {
    return IntTable<set<V>>::forEach(function, terminator);
  }

  int size() const override { return IntTable<set<V>>::size(); };
};
