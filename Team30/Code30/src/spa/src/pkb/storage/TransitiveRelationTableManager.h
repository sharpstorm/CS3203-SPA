#pragma once
#include <memory>
#include <set>

#include "RelationTableManager.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"
using pkb::Predicate;
using std::make_unique;
using std::unique_ptr;

/**
 * Table manager for transitive relations, R*(left, right) where args are type T.
 * R*(left, right) is true if
 * - R(left, right) or
 * - R(left, a) and R*(a, right) for some a
 */
template <typename T>
class TransitiveRelationTableManager : public RelationTableManager<T, T> {
 public:
  TransitiveRelationTableManager(IBaseSetTable<T, T> *table,
                                 IBaseSetTable<T, T> *reverseTable)
      : RelationTableManager<T, T>(table, reverseTable) {}

  /**
   * Get set of right where R*(left, right) is true, given left.
   */
  set<T> getByFirstArg(T left) const override {
    auto result = set<T>({});
    getByFirstArgTHelper(left, &result);
    return result;
  }

  /**
   * Get set of left where R*(left, right) is true, given right.
   */
  set<T> getBySecondArg(T right) const override {
    auto result = set<T>({});
    getBySecondArgTHelper(right, &result);
    return result;
  }

  /**s
   * Find R*(left, right) where left is in the given leftValues and right
   * satisfies rightPredicate.
   */
  QueryResultPtr<T, T> query(
      set<T> leftValues, Predicate<T> rightPredicate,
      QueryResultBuilder<T, T> *resultBuilder) const override {
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
   * Find R*(left, right) where right is in the given rightValues and left
   * satisfies leftPredicate.
   */
  QueryResultPtr<T, T> query(
      Predicate<T> leftPredicate, set<T> rightValues,
      QueryResultBuilder<T, T> *resultBuilder) const override {
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
   * Find R*(left, right) given left and right satisfies rightPredicate.
   */
  QueryResultPtr<T, T> query(
      T left, Predicate<T> rightPredicate,
      QueryResultBuilder<T, T> *resultBuilder) const override {
    return query(set<T>({left}), rightPredicate, resultBuilder);
  }

  /**
   * Find R*(left, right) given right and left satisfies leftPredicate.
   */
  QueryResultPtr<T, T> query(
      Predicate<T> leftPredicate, T right,
      QueryResultBuilder<T, T> *resultBuilder) const override {
    return query(leftPredicate, set<T>({right}), resultBuilder);
  }

 private:
  void getByFirstArgTHelper(T left, set<T> *allResults) const {
    auto result = this->table->get(left);
    for (auto r : result) {
      if (allResults->find(r) != allResults->end()) {
        continue;
      }
      allResults->insert(r);
      getByFirstArgTHelper(r, allResults);
    }
    return;
  }

  void getBySecondArgTHelper(T right, set<T> *allResults) const {
    auto result = this->reverseTable->get(right);
    for (auto r : result) {
      if (allResults->find(r) != allResults->end()) {
        continue;
      }
      allResults->insert(r);
      getBySecondArgTHelper(r, allResults);
    }
    return;
  }
};
