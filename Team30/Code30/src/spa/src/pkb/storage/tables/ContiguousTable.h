#pragma once

#include <vector>
#include "BaseTable.h"

using std::vector;

template<typename V>
class ContiguousTable {
 protected:
  vector<V> table;
  void resizeIfExceed(int key) {
    if (key >= table.size()) {
      table.resize(key * 2);
    }
  }

 public:
  explicit ContiguousTable(int size = 1) : table(size) {}

  void set(int key, V value) {
    resizeIfExceed(key);
    table[key] = value;
  }

  V get(int key) const {
    if (key < table.size()) {
      return table.at(key);
    }
    return V();
  }
};
