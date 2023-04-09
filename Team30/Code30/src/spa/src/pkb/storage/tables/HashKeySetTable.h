/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "HashKeyTable.h"
#include "IBaseSetTable.h"
#include "pkb/storage/iterators/SetIterator.h"

using std::unordered_set, std::make_unique;

template <typename K, typename V>
class HashKeySetTable : public IBaseSetTable<K, V>,
                        public HashKeyTable<K, unordered_set<V>> {
 public:
  HashKeySetTable() : HashKeyTable<K, unordered_set<V>>() {}

  void insert(K key, V value) override {
    assert(key != K());
    assert(value != V());

    this->table[key].insert(value);
  }

  const unordered_set<V>& get(K key) const override {
    return HashKeyTable<K, unordered_set<V>>::get(key);
  }

  int size() const override {
    return HashKeyTable<K, unordered_set<V>>::size();
  };

  bool isEmpty() const override {
    return HashKeyTable<K, unordered_set<V>>::isEmpty();
  }

  bool contains(K key, V value) const override {
    return get(key).count(value);
  };

  bool containsKey(K key) const override { return !get(key).empty(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(K key) override {
    return make_unique<SetIterator<V>>(
        HashKeyTable<K, unordered_set<V>>::get(key));
  }
};
