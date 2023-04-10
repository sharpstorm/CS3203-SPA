/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

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
 * Table manager for relation, R(leftArg, rightArg), where args are type K and V
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

  void insert(K leftArg, V rightArg) {
    table->insert(leftArg, rightArg);
    reverseTable->insert(rightArg, leftArg);
  }

  virtual unique_ptr<IBaseIterator<V>> getRightValIter(K leftArg) const {
    return table->getValueIterator(leftArg);
  }

  virtual unique_ptr<IBaseIterator<K>> getLeftValIter(V rightArg) const {
    return reverseTable->getValueIterator(rightArg);
  }

  /**
   * Find R(leftArg, rightArg) where leftArg and rightArg are given values.
   */
  virtual QueryResultPtr<K, V> query(
      K leftArg, V rightArg, QueryResultBuilder<K, V> *resultBuilder) const {
    if (table->contains(leftArg, rightArg)) {
      resultBuilder->add(leftArg, rightArg);
    }

    return resultBuilder->getResult();
  }

  /**
   * Find R(leftArg, rightArg) where leftArg is in the given arg1Values and
   * rightArg satisfies arg2Predicate.
   */
  virtual QueryResultPtr<K, V> query(
      const unordered_set<K> &arg1Values, Predicate<V> arg2Predicate,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto leftArg : arg1Values) {
      auto it = getRightValIter(leftArg);
      V rightArg;
      while ((rightArg = it->getNext()) != V()) {
        if (arg2Predicate(rightArg)) {
          resultBuilder->add(leftArg, rightArg);
        }
      }
    }

    return resultBuilder->getResult();
  }

  /**
   * Find R(leftArg, rightArg) where rightArg is in the given arg2Values and
   * leftArg satisfies arg1Predicate.
   */
  virtual QueryResultPtr<K, V> query(
      Predicate<K> arg1Predicate, const unordered_set<V> &arg2Values,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto rightArg : arg2Values) {
      auto it = getLeftValIter(rightArg);
      K leftArg;
      while ((leftArg = it->getNext()) != K()) {
        if (arg1Predicate(leftArg)) {
          resultBuilder->add(leftArg, rightArg);
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
    for (auto leftArg : arg1Values) {
      if (table->containsKey(leftArg)) {
        resultBuilder->addLeft(leftArg);
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
    for (auto rightArg : arg2Values) {
      if (reverseTable->containsKey(rightArg)) {
        resultBuilder->addRight(rightArg);
      }
    }
    return resultBuilder->getResult();
  }
};
