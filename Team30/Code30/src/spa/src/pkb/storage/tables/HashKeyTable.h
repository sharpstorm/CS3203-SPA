#pragma once

#include <unordered_map>
#include "IBaseTable.h"

using std::unordered_map;

template<typename K, typename V>
class HashKeyTable : public IBaseTable<K, V> {
 protected:
  unordered_map<K, V> table;

 public:
  HashKeyTable() : table() {}

  void set(K key, V value) override {
    table[key] = value;
  }

  V get(K key) const override {
    auto iter = table.find(key);
    if (iter != table.end()) {
      return iter->second;
    }
    return V();
  }
};
