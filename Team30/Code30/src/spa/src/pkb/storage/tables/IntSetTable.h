#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "IBaseSetTable.h"
#include "IntTable.h"
#include "pkb/storage/iterators/SetIterator.h"

using std::set, std::make_unique;

template <typename V>
class IntSetTable : public IBaseSetTable<int, V>, public IntTable<set<V>> {
 public:
  using IntTable<set<V>>::IntTable;

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    this->table[key].insert(value);
  }

  const set<V>& get(int key) const override {
    return IntTable<set<V>>::get(key);
  }

  int size() const override { return IntTable<set<V>>::size(); };

  bool isEmpty() const override { return IntTable<set<V>>::isEmpty(); }

  bool contains(int key, V value) const override {
    return get(key).count(value);
  };

  bool containsKey(int key) const override { return !get(key).empty(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(int key) {
    return make_unique<SetIterator<V>>(IntTable<set<V>>::get(key));
  }
};
