#pragma once

#include <cassert>
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
    assert(key != K());
    assert(value != V());

    table[key] = value;
  }

  V get(K key) const override {
    assert(key != K());

    auto iter = table.find(key);
    if (iter != table.end()) {
      return iter->second;
    }
    return V();
  }

  typename unordered_map<K, V>::const_iterator begin() const {
    return table.begin();
  }

  typename unordered_map<K, V>::const_iterator end() const {
    return table.end();
  }
};
