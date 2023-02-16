#pragma once

#include <memory>
#include <unordered_set>

#include "../../common/Types.h"
#include "../predicates/Predicate.h"
#include "tables/IBaseSetTable.h"
#include "tables/IBaseTable.h"

using std::shared_ptr;
using std::unordered_set;

/**
 * Table manager for entity.
 * Stores mapping of K -> V and V-> Set<K>. Provides insert
 * and get by key / value functionalities.
 */
template <typename K, typename V>
class EntityTableManager {
 protected:
  shared_ptr<IBaseTable<K, V>> table;            // maps K -> V
  shared_ptr<IBaseSetTable<V, K>> reverseTable;  // maps V -> set<K>
  unordered_set<K> allKeys;
  unordered_set<V> allValues;

 public:
  EntityTableManager(shared_ptr<IBaseTable<K, V>> table,
                     shared_ptr<IBaseSetTable<V, K>> reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(K arg1, V arg2) {
    table->set(arg1, arg2);
    reverseTable->set(arg2, arg1);
    allKeys.insert(arg1);
    allValues.insert(arg2);
  }

  void insertFromTo(K startNum, K endNum, V arg2) {
    allValues.insert(arg2);
    for (int i = startNum; i < endNum + 1; ++i) {
      allKeys.insert(i);
      table->set(i, arg2);
      reverseTable->set(arg2, i);
    }
  }

  V getByKey(K arg1) const { return table->get(arg1); }

  unordered_set<K> getByValue(V arg2) const { return reverseTable->get(arg2); }

  unordered_set<K> getAllKeys() const { return allKeys; }
  unordered_set<V> getAllValues() const { return allValues; }
};
