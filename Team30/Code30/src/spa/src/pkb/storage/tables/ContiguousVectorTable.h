#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "ContiguousTable.h"
#include "IBaseSetTable.h"
#include "pkb/storage/iterators/VectorIterator.h"

using std::vector;

template <typename V>
class ContiguousVectorTable : public IBaseSetTable<int, V>,
                              public ContiguousTable<vector<V>> {
 public:
  explicit ContiguousVectorTable(int size = 1)
      : ContiguousTable<vector<V>>(size) {}

  void insert(int key, V value) {
    assert(key != 0);
    assert(value != V());

    this->resizeIfExceed(key);
    vector<V>& values = this->table[key];
    auto capacity = values.capacity();
    if (values.size() >= capacity) {
      values.reserve(capacity * 2);
    }
    values.push_back(value);
  }

  const vector<V>& get(int key) const {
    return ContiguousTable<vector<V>>::get(key);
  }

  int size() const override { return ContiguousTable<vector<V>>::size(); };

  bool isEmpty() const override {
    return ContiguousTable<vector<V>>::isEmpty();
  }

  bool contains(int key, V value) const override {
    auto values = get(key);
    return std::binary_search(values.begin(), values.end(), value);
  }

  bool containsKey(int key) const override { return !get(key).empty(); }

  unique_ptr<IBaseIterator<V>> getValueIterator(int key) {
    return make_unique<VectorIterator<V>>(
        &ContiguousTable<vector<V>>::get(key));
  }

  V getFirstValue(int key) const {
    auto values = get(key);
    if (values.empty()) {
      return V();
    } else {
      return values.front();
    }
  }

  V getLastValue(int key) const {
    auto values = get(key);
    if (values.empty()) {
      return V();
    } else {
      return values.back();
    }
  }

};
