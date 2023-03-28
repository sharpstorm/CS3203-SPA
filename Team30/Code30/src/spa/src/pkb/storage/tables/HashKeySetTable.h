#pragma once

#include <unordered_map>
#include <set>
#include <cassert>
#include "IBaseSetTable.h"
#include "HashKeyTable.h"

using std::set;

template<typename K, typename V>
class HashKeySetTable : public IBaseSetTable<K, V>,
                        public HashKeyTable<K, set<V>> {
 public:
  HashKeySetTable() : HashKeyTable<K, set<V>>() {}

  void insert(K key, V value) override {
    assert(key != K());
    assert(value != V());

    this->table[key].insert(value);
  }

  const set<V>& get(K key) const override {
    assert(key != K());

    return HashKeyTable<K, set<V>>::get(key);
  }
  void begin() const override {}
  void end() const override {}
};
