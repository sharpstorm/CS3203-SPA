/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include <cassert>
#include <map>
#include <memory>

#include "IBaseTable.h"
#include "pkb/storage/iterators/MapIterator.h"

using std::map, std::make_unique;

template <typename V>
class IntTable : public IBaseTable<int, V> {
 protected:
  map<int, V> table;
  static inline const V emptyValue = V();

 public:
  IntTable() : table() {}

  void insert(int key, V value) override {
    assert(key != 0);
    assert(value != V());

    table[key] = value;
  }

  const V& get(int key) const override {
    assert(key != 0);

    if (table.find(key) != table.end()) {
      return table.at(key);
    }
    return emptyValue;
  }

  static const V& getEmptyValue() { return emptyValue; }

  int size() const override { return table.size(); };

  bool isEmpty() const override { return table.empty(); }

  unique_ptr<IBaseTableIterator<int, V>> getTableIterator() override {
    return make_unique<MapIterator<int, V>>(table);
  }
};
