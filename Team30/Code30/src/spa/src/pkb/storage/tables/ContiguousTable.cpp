#include "ContiguousTable.h"

template <typename V>
ContiguousTable<V>::ContiguousTable() : table() {}

template <typename V>
void ContiguousTable<V>::set(int key, V value) {
  table[key].insert(value);
}

template <typename V>
std::unordered_set<V> ContiguousTable<V>::get(int key) {
  return table[key];
}
