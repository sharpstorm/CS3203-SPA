#pragma once

#include <cassert>
#include <map>

#include "IBaseTable.h"

using std::map;

template <typename V>
class IntTable {
 protected:
  map<int, V> table;
  static inline const V emptyValue = V();

 public:
  IntTable() : table() {}

  virtual void insert(int key, V value) {
    assert(key != 0);
    assert(value != V());

    table[key] = value;
  }

  virtual const V& get(int key) const {
    assert(key != 0);

    if (table.find(key) != table.end()) {
      return table.at(key);
    }
    return emptyValue;
  }

  virtual typename map<int, V>::const_iterator begin() const {
    return table.begin();
  }

  virtual typename map<int, V>::const_iterator end() const {
    return table.end();
  }

  static const V& getEmptyValue() { return emptyValue; }
};
