#pragma once

#include <cassert>
#include <set>
#include <unordered_map>

#include "HashKeyTable.h"
#include "IBaseSetTable.h"

using std::set;

template <typename K, typename V>
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
    return HashKeyTable<K, set<V>>::get(key);
  }

  void forEach(pkb::Function<K, set<V>> function,
               pkb::Terminator<K, set<V>> terminator) const override {
    return HashKeyTable<K, set<V>>::forEach(function, terminator);
  }

  int size() const override {
    return HashKeyTable<K, set<V>>::size();
  };
};
