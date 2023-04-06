#pragma once

#include <cassert>
#include <memory>
#include <set>

#include "ContiguousTable.h"
#include "IBaseSetTable.h"
#include "pkb/storage/iterators/SetIterator.h"

using std::set, std::unique_ptr, std::make_unique;

template <typename V>
class ContiguousSetTable : public IBaseSetTable<int, V>,
                           public ContiguousTable<set<V>> {
 public:
  explicit ContiguousSetTable(int size = 1) : ContiguousTable<set<V>>(size) {}

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    this->resizeIfExceed(key);
    this->table[key].insert(value);
  }

  const set<V>& get(int key) const { return ContiguousTable<set<V>>::get(key); }

  int size() const override { return ContiguousTable<set<V>>::size(); };

  bool isEmpty() const override { return ContiguousTable<set<V>>::isEmpty(); }

  bool contains(int key, V value) const override {
    return get(key).count(value);
  };

  bool containsKey(int key) const override { return !get(key).empty(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(int key) {
    return make_unique<SetIterator<V>>(ContiguousTable<set<V>>::get(key));
  }
};
