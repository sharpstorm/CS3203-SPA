#pragma once

#include <memory>
#include <set>

#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"
#include "tables/IBaseSetTable.h"

using pkb::Predicate;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

/**
 * Table manager for relation, R(left, right), where args are type K and V
 * respectively. Stores mapping of K -> Set<V> and V-> Set<K>. Provides insert
 * and query functionalities.
 */

template <typename K, typename V>
class RelationTableManager {
 protected:
  IBaseSetTable<K, V> *table;         // maps K -> set<V>
  IBaseSetTable<V, K> *reverseTable;  // maps V -> set<K>

 public:
  RelationTableManager(IBaseSetTable<K, V> *table,
                       IBaseSetTable<V, K> *reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(K left, V right) {
    table->insert(left, right);
    reverseTable->insert(right, left);
  }

  /**
   * Get set of right where R(left, right) is true, given left value.
   */
  virtual set<V> getByFirstArg(K left) const { return table->get(left); }

  /**
   * Get set of left where R(left, right) is true, given right value.
   */
  virtual set<K> getBySecondArg(V right) const {
    return reverseTable->get(right);
  }

  /**
   * Find R(left, right) where left is in the given leftValues and right satisfies
   * rightPredicate.
   */
  virtual QueryResultPtr<K, V> query(
      set<K> leftValues, Predicate<V> rightPredicate,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto left : leftValues) {
      auto rightValues = getByFirstArg(left);
      for (auto right : rightValues) {
        if (rightPredicate(right)) {
          resultBuilder->add(left, right);
        }
      }
    }

    return resultBuilder->getResult();
  }

  /**
   * Find R(left, right) where right is in the given rightValues and left satisfies
   * leftPredicate.
   */
  virtual QueryResultPtr<K, V> query(
      Predicate<K> leftPredicate, set<V> rightValues,
      QueryResultBuilder<K, V> *resultBuilder) const {
    for (auto right : rightValues) {
      auto leftValues = getBySecondArg(right);
      for (auto left : leftValues) {
        if (leftPredicate(left)) {
          resultBuilder->add(left, right);
        }
      }
    }
    return resultBuilder->getResult();
  }

  /**
   * Find R(left, right) given left and right satisfies rightPredicate.
   */
  virtual QueryResultPtr<K, V> query(
      K left, Predicate<V> rightPredicate,
      QueryResultBuilder<K, V> *resultBuilder) const {
    return query(set<K>({left}), rightPredicate, resultBuilder);
  }

  /**
   * Find R(left, right) given right and left satisfies leftPredicate.
   */
  virtual QueryResultPtr<K, V> query(
      Predicate<K> leftPredicate, V right,
      QueryResultBuilder<K, V> *resultBuilder) const {
    return query(leftPredicate, set<V>({right}), resultBuilder);
  }
};
