/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include <cassert>
#include <memory>
#include <vector>

#include "IBaseTable.h"
#include "pkb/storage/iterators/ContiguousTableIterator.h"

using std::vector, std::make_unique, std::unique_ptr;

template <typename V>
class ContiguousTable : public IBaseTable<int, V> {
 protected:
  vector<V> table;
  static inline const V emptyValue = V();
  void resizeIfExceed(int key) {
    if (key >= table.size()) {
      table.resize(key * 2);
    }
  }

 public:
  explicit ContiguousTable(int size = 1) : table(size) {}

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    resizeIfExceed(key);
    table[key] = value;
  }

  const V& get(int key) const override {
    assert(key != 0);

    if (key < table.size()) {
      return table.at(key);
    }
    return emptyValue;
  }

  static const V& getEmptyValue() { return emptyValue; }

  int size() const override { return table.size(); };

  bool isEmpty() const override { return table.empty(); }

  unique_ptr<IBaseTableIterator<int, V>> getTableIterator() override {
    return make_unique<ContiguousTableIterator<V>>(table);
  }
};
