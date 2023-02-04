#pragma once

#include <unordered_map>
#include <unordered_set>

#include "BaseTable.h"
// map K -> set<V>
template <typename K, typename V>
class HashKeyTable : public BaseTable<K, V> {
 private:
  std::unordered_map<K, std::unordered_set<V>> table;

 public:
  HashKeyTable();

  void set(K key, V value);

  std::unordered_set<V> get(K key);
};
