#pragma once

#include <cassert>
#include <unordered_map>
#include "IBaseTable.h"

using std::unordered_map;

template<typename K, typename V>
class HashKeyTable : public IBaseTable<K, V> {
 protected:
  unordered_map<K, V> table;
  static inline const V emptyValue = V();

 public:
  HashKeyTable() : table() {}

  void insert(K key, V value) override {
    assert(key != K());
    assert(value != V());

    table[key] = value;
  }

  const V& get(K key) const override {
    assert(key != K());

    auto iter = table.find(key);
    if (iter != table.end()) {
      return iter->second;
    }
    return emptyValue;
  }

  void begin() const override {}
  void end() const override {}

  static const V& getEmptyValue() {
    return emptyValue;
  }
};
