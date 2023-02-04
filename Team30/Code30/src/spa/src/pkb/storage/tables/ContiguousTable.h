#pragma once

#include <unordered_set>

#include "BaseTable.h"
// map int -> set<V>
template <typename V>
class ContiguousTable : public BaseTable<int, V> {
 private:
  std::unordered_set<V> table[10];

 public:
  ContiguousTable();

  void set(int key, V value);

  std::unordered_set<V> get(int key);
};
