#pragma once

#include <unordered_set>

#include "BaseTable.h"
// map int -> set<V>
template <typename V>
class ContiguousTable : public BaseTable<int, V> {
 private:
  std::unordered_set<V> table[10];

 public:
  ContiguousTable() : table() {}

  void set(int key, V value) override { table[key].insert(value); }

  std::unordered_set<V> get(int key) const override { return table[key]; }
};
