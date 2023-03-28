#pragma once

#include <cassert>
#include <set>

#include "IBaseSetTable.h"
#include "IntTable.h"

using std::set;

template <typename V>
class IntSetTable : public IntTable<set<V>> {
 public:
  using IntTable<set<V>>::IntTable;

  void insert(int key, V value) {
    assert(key != 0);
    assert(value != V());

    this->table[key].insert(value);
  }
};
