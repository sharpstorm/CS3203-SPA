#pragma once

#include <memory>
#include <set>

#include "IStorage.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "tables/IBaseSetTable.h"

using pkb::Predicate;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

/**
 * Table manager for relation, R(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> Set<V> and V-> Set<K>. Provides insert
 * and query functionalities.
 */

template <typename K, typename V>
class RelationTableManager : public IStorage<K, V> {
 protected:
  IBaseSetTable<K, V> *table;         // maps K -> set<V>
  IBaseSetTable<V, K> *reverseTable;  // maps V -> set<K>

 public:
  RelationTableManager(IBaseSetTable<K, V> *table,
                       IBaseSetTable<V, K> *reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(K arg1, V arg2) {
    table->insert(arg1, arg2);
    reverseTable->insert(arg2, arg1);
  }

  virtual unique_ptr<IBaseIterator<V>> getRightValIter(K leftArg) const {
    return table->getValueIterator(leftArg);
  }

  virtual unique_ptr<IBaseIterator<K>> getLeftValIter(V rightArg) const {
    return reverseTable->getValueIterator(rightArg);
  }

  /**
   * Find R(arg1, arg2) where arg1 and arg2 are given values.
   */
  virtual QueryResultPtr<K, V> query(K arg1, V arg2) const {
    QueryResult<K, V> result;
    if (table->contains(arg1, arg2)) {
      result.add(arg1, arg2);
    }

    return make_unique<QueryResult<K, V>>(result);
  }

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(set<K> arg1Values,
                                     Predicate<V> arg2Predicate) const {
    QueryResult<K, V> result;
    for (auto arg1 : arg1Values) {
      auto it = getRightValIter(arg1);
      V arg2;
      while ((arg2 = it->getNext()) != V()) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }

    return make_unique<QueryResult<K, V>>(result);
  }

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate,
                                     set<V> arg2Values) const {
    QueryResult<K, V> result;
    for (auto arg2 : arg2Values) {
      auto it = getLeftValIter(arg2);
      K arg1;
      while ((arg1 = it->getNext()) != K()) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return make_unique<QueryResult<K, V>>(result);
  }

  /**
   * Find R(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(K arg1, Predicate<V> arg2Predicate) const {
    return query(set<K>({arg1}), arg2Predicate);
  }

  /**
   * Find R(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(Predicate<K> arg1Predicate, V arg2) const {
    return query(arg1Predicate, set<V>({arg2}));
  }

  /**
   * Return non-empty result if at least one such relation
   */
  virtual QueryResultPtr<K, V> hasRelation() const {
    QueryResult<K, V> result;
    if (!table->isEmpty()) {
      result.isEmpty = false;
    }
    return make_unique<QueryResult<K, V>>(result);
  }

  /**
   * Right side wildcard.
   */
  virtual QueryResultPtr<K, V> rightWildcardQuery(
      const set<K> &arg1Values) const {
    QueryResult<K, V> result;
    for (auto arg1 : arg1Values) {
      if (table->containsKey(arg1)) {
        result.firstArgVals.insert(arg1);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<K, V>>(result);
  }

  /**
   * Left side wildcard.
   */
  virtual QueryResultPtr<K, V> leftWildcardQuery(
      const set<V> &arg2Values) const {
    QueryResult<K, V> result;
    for (auto arg2 : arg2Values) {
      if (reverseTable->containsKey(arg2)) {
        result.secondArgVals.insert(arg2);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<K, V>>(result);
  }
};
