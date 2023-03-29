#pragma once

#include <cassert>
#include <set>

#include "IBaseSetTable.h"
#include "IntTable.h"

using std::set;

template <typename V>
class IntSetTable : public IntTable<set<V>> {
 private:
  static inline const V emptyValue = V();
 public:
  using IntTable<set<V>>::IntTable;

  void insert(int key, V value) {
    assert(key != 0);
    assert(value != V());

    this->table[key].insert(value);
  }

  const V& getFirstValue(StmtValue key) const {
    auto values = IntTable::get(key);
    if (values.empty()) {
      return IntSetTable::emptyValue;
    }
    return *(values.begin());
  }

  const V& getLastValue(StmtValue key) const {
    auto values = IntTable::get(key);
    if (values.empty()) {
      return IntSetTable::emptyValue;
    }
    return *(values.rbegin());
  }
};
