#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "ContiguousTable.h"
#include "IBaseSetTable.h"

using std::vector;

template <typename V>
class ContiguousVectorTable : public ContiguousTable<vector<V>> {
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

  void forEach(pkb::Function<int, vector<V>>& function) const {
    return ContiguousTable<vector<V>>::forEach(function);
  }

  int size() const override { return ContiguousTable<vector<V>>::size(); };

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

  bool contains(int key, V value) const {
    auto values = get(key);
    return std::binary_search(values.begin(), values.end(), value);
  }

  bool containsKey(int key) const { return !get(key).empty(); }
};
