#pragma once

#include <memory>
#include <unordered_set>

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "tables/IBaseSetTable.h"

using std::shared_ptr;
using std::unordered_set;
using pkb::Predicate;

/**
 * Table manager for relation, R(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> Set<V> and V-> Set<K>. Provides insert
 * and query functionalities.
 */

template<typename K, typename V>
class RelationTableManager {
 protected:
  IBaseSetTable<K, V> *table;         // maps K -> set<V>
  IBaseSetTable<V, K> *reverseTable;  // maps V -> set<K>


 public:
  RelationTableManager(
      IBaseSetTable<K, V> *table,
      IBaseSetTable<V, K> *reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(K arg1, V arg2) {
    table->set(arg1, arg2);
    reverseTable->set(arg2, arg1);
  }

  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  virtual unordered_set<V> getByFirstArg(K arg1) const {
    return table->get(
        arg1);
  }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  virtual unordered_set<K> getBySecondArg(V arg2) const {
    return reverseTable->get(arg2);
  }

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  virtual QueryResult<K, V> query(
      unordered_set<K> arg1Values, Predicate<V> arg2Predicate) const {
    QueryResult<K, V> result;
    for (auto arg1 : arg1Values) {
      auto arg2Values = table->get(arg1);
      for (auto arg2 : arg2Values) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }
    return result;
  }

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResult<K, V> query(
      Predicate<K> arg1Predicate, unordered_set<V> arg2Values) const {
    QueryResult<K, V> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = reverseTable->get(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return result;
  }

  /**
   * Find R(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  virtual QueryResult<K, V> query(K arg1, Predicate<V> arg2Predicate) const {
    return query(unordered_set<K>({arg1}), arg2Predicate);
  }

  /**
   * Find R(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  virtual QueryResult<K, V> query(Predicate<K> arg1Predicate, V arg2) const {
    return query(arg1Predicate, unordered_set<V>({arg2}));
  }
};
