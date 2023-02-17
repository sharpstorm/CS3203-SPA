#pragma once

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include "IBaseSetTable.h"
#include "HashKeyTable.h"

using std::unordered_set;

template<typename K, typename V>
class HashKeySetTable : public IBaseSetTable<K, V>,
                        public HashKeyTable<K, unordered_set<V>> {
 public:
  HashKeySetTable() : HashKeyTable<K, unordered_set<V>>() {}

  void set(K key, V value) override {
    assert(key != K());
    assert(value != V());

    this->table[key].insert(value);
  }

  unordered_set<V> get(K key) const override {
    assert(key != K());

    return HashKeyTable<K, unordered_set<V>>::get(key);
  }
};
