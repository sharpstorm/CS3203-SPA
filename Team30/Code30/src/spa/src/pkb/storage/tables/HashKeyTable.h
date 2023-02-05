#pragma once

#include <unordered_map>
#include <unordered_set>

#include "BaseTable.h"

template <typename K, typename V>
class HashKeyTable : public BaseTable<K, V> {
 private:
  std::unordered_map<K, std::unordered_set<V>> table;

 public:
  HashKeyTable() : table() {}

  void set(K key, V value) override { table[key].insert(value); }

  std::unordered_set<V> get(K key) const override {
    auto iter = table.find(key);
    if (iter != table.end()) {
      return iter->second;
    }
    return {};
  }
};
