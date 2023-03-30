#pragma once

#include <cassert>
#include <map>

#include "IBaseTable.h"

using std::map;

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

  void forEach(pkb::Function<int, V> function,
               pkb::Terminator<int, V> terminator) const override {
    for (auto it = table.begin(); it != table.end(); it++) {
      const int& key = it->first;
      const V& values = it->second;
      if (terminator != nullptr && terminator(key, values)) {
        return;
      }
      function(key, values);
    }
  }

  int size() const override { return table.size(); };
};
