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
 * Table manager for entity, E(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> V and V-> Set<K>. Provides insert
 * and get by key / value functionalities.
 */
template <typename K, typename V>
class EntityTableManager {
 protected:
  shared_ptr<IBaseTable<K, V>> table;            // maps K -> V
  shared_ptr<IBaseSetTable<V, K>> reverseTable;  // maps V -> set<K>
  unordered_set<K> allValues;

 public:
  EntityTableManager(shared_ptr<IBaseTable<K, V>> table,
                     shared_ptr<IBaseSetTable<V, K>> reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(K arg1, V arg2) {
    table->set(arg1, arg2);
    reverseTable->set(arg2, arg1);
    allValues.insert(arg1);
  }

  /**
   * Get arg2 given arg1 value.
   */
  V getByKey(K arg1) const { return table->get(arg1); }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  unordered_set<K> getByValue(V arg2) const { return reverseTable->get(arg2); }

  unordered_set<K> getAllValues() const { return allValues; }
};
