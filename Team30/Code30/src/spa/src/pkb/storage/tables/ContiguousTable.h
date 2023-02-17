#pragma once

#include <vector>
#include <cassert>
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
    assert(key != 0);
    assert(value != V());

    resizeIfExceed(key);
    table[key] = value;
  }

  V get(int key) const override {
    assert(key != 0);

    if (key < table.size()) {
      return table.at(key);
    }
    return V();
  }
};
