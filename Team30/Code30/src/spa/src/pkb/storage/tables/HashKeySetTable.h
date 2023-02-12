#pragma once

#include <unordered_map>
#include <unordered_set>
#include "IBaseSetTable.h"
#include "HashKeyTable.h"

using std::unordered_set;

template<typename K, typename V>
class HashKeySetTable : public IBaseSetTable<K, V>,
                        public HashKeyTable<K, unordered_set<V>> {
 public:
  HashKeySetTable() : HashKeyTable<K, unordered_set<V>>() {}

  void set(K key, V value) override {
    this->table[key].insert(value);
  }

  unordered_set<V> get(K key) const override {
    return HashKeyTable<K, unordered_set<V>>::get(key);
  }
};
