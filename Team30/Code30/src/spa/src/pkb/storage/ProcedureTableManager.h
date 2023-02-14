#pragma once

#include <memory>
#include <unordered_set>
#include <utility>

#include "../../common/Types.h"
#include "../predicates/Predicate.h"
#include "tables/ContiguousTable.h"
#include "tables/HashKeyTable.h"

using std::shared_ptr;
using std::unordered_set;

/**
 * Table manager for procedure, P(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> pair<V,V> and V-> K. Provides insert
 * and get by key / value functionalities.
 */
template <typename K, typename V>
class ProcedureTableManager {
 protected:
  shared_ptr<HashKeyTable<K, pair<V, V>>> table;  // maps K -> pair<V,V>
  shared_ptr<ContiguousTable<K>> reverseTable;    // maps V -> set<K>

 public:
  ProcedureTableManager(shared_ptr<HashKeyTable<K, pair<V, V>>> table,
                        shared_ptr<ContiguousTable<K>> reverseTable)
      : table(table), reverseTable(reverseTable) {}

  /**
   *
   * @param arg1 ProcedureName
   * @param arg2 StartlineNum
   * @param arg3 EndLineNum
   */
  void insert(K arg1, V arg2, V arg3) {
    table->set(arg1, std::make_pair(arg2, arg3));

    for (int i = arg2; i < arg3 + 1; ++i) {
      reverseTable->set(i, arg1);
    }
  }

  /**
   * Get Pair<arg2,arg3> given arg1 value.
   */
  pair<V, V> getByKey(K arg1) const { return table->get(arg1); }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  K getByValue(V arg2) const { return reverseTable->get(arg2); }
};
