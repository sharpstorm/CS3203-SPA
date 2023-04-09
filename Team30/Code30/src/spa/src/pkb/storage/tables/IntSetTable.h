/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include <cassert>
#include <memory>
#include <unordered_set>

#include "IBaseSetTable.h"
#include "IntTable.h"
#include "pkb/storage/iterators/SetIterator.h"

using std::unordered_set, std::make_unique;

template <typename V>
class IntSetTable : public IBaseSetTable<int, V>,
                    public IntTable<unordered_set<V>> {
 public:
  using IntTable<unordered_set<V>>::IntTable;

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    this->table[key].insert(value);
  }

  const unordered_set<V>& get(int key) const override {
    return IntTable<unordered_set<V>>::get(key);
  }

  int size() const override { return IntTable<unordered_set<V>>::size(); };

  bool isEmpty() const override {
    return IntTable<unordered_set<V>>::isEmpty();
  }

  bool contains(int key, V value) const override {
    return get(key).count(value);
  };

  bool containsKey(int key) const override { return !get(key).empty(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(int key) override {
    return make_unique<SetIterator<V>>(IntTable<unordered_set<V>>::get(key));
  }
};
