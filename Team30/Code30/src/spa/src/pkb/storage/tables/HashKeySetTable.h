#pragma once

#include <cassert>
#include <set>
#include <unordered_map>

#include "HashKeyTable.h"
#include "IBaseSetTable.h"
#include "pkb/storage/iterators/SetIterator.h"

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

  const set<V>& get(K key) const { return HashKeyTable<K, set<V>>::get(key); }

  int size() const override { return HashKeyTable<K, set<V>>::size(); };

  bool isEmpty() const override { return HashKeyTable<K, set<V>>::isEmpty(); }

  bool contains(K key, V value) const override {
    return get(key).count(value);
  };

  bool containsKey(K key) const override { return !get(key).empty(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(K key) {
    return make_unique<SetIterator<V>>(&HashKeyTable<K, set<V>>::get(key));
  }
};
