#pragma once

#include <vector>
#include "IBaseTable.h"

using std::vector;

template<typename V>
class ContiguousTable : public IBaseTable<int, V> {
 protected:
  vector<V> table;
  void resizeIfExceed(int key) {
    if (key >= table.size()) {
      table.resize(key * 2);
    }
  }

 public:
  explicit ContiguousTable(int size = 1) : table(size) {}

  void set(int key, V value) override {
    resizeIfExceed(key);
    table[key] = value;
  }

  V get(int key) const override {
    if (key < table.size()) {
      return table.at(key);
    }
    return V();
  }
};
