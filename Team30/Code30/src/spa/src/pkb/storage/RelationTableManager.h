#pragma once

#include <memory>
#include <unordered_set>

#include "IStorage.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"
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
  IBaseSetTable<K, V> *table;
  IBaseSetTable<V, K> *reverseTable;

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
  virtual QueryResultPtr<K, V> query(
      K arg1, V arg2, QueryResultBuilder<K, V> *resultBuilder) const {
    if (table->contains(arg1, arg2)) {
      resultBuilder->add(arg1, arg2);
    }

    return resultBuilder->getResult();
  }

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(
      const unordered_set<K> &arg1Values, Predicate<V> arg2Predicate,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto arg1 : arg1Values) {
      auto it = getRightValIter(arg1);
      V arg2;
      while ((arg2 = it->getNext()) != V()) {
        if (arg2Predicate(arg2)) {
          resultBuilder->add(arg1, arg2);
        }
      }
    }

    return resultBuilder->getResult();
  }

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(
      Predicate<K> arg1Predicate, const unordered_set<V> &arg2Values,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto arg2 : arg2Values) {
      auto it = getLeftValIter(arg2);
      K arg1;
      while ((arg1 = it->getNext()) != K()) {
        if (arg1Predicate(arg1)) {
          resultBuilder->add(arg1, arg2);
        }
      }
    }
    return resultBuilder->getResult();
  }

  /**
   * Return non-empty result if at least one such relation
   */
  virtual QueryResultPtr<K, V> hasRelation(
      QueryResultBuilder<K, V> *resultBuilder) const {
    if (!table->isEmpty()) {
      resultBuilder->setIsNotEmpty();
    }
    return resultBuilder->getResult();
  }

  /**
   * Right side wildcard.
   */
  virtual QueryResultPtr<K, V> rightWildcardQuery(
      const unordered_set<K> &arg1Values,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto arg1 : arg1Values) {
      if (table->containsKey(arg1)) {
        resultBuilder->addLeft(arg1);
      }
    }
    return resultBuilder->getResult();
  }

  /**
   * Left side wildcard.
   */
  virtual QueryResultPtr<K, V> leftWildcardQuery(
      const unordered_set<V> &arg2Values,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto arg2 : arg2Values) {
      if (reverseTable->containsKey(arg2)) {
        resultBuilder->addRight(arg2);
      }
    }
    return resultBuilder->getResult();
  }
};
