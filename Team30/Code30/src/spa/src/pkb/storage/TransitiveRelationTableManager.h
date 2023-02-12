#pragma once
#include <memory>
#include <unordered_set>

#include "RelationTableManager.h"
/**
 * Table manager for transitive relations, R*(arg1, arg2) where args are type T.
 * R*(arg1, arg2) is true if
 * - R(arg1, arg2) or
 * - R(arg1, a) and R*(a, arg2) for some a
 */
template<typename T>
class TransitiveRelationTableManager : public RelationTableManager<T, T> {
 public:
  TransitiveRelationTableManager(shared_ptr<IBaseSetTable<T, T>> table,
                                 shared_ptr<IBaseSetTable<T, T>> reverseTable)
      : RelationTableManager<T, T>(table, reverseTable) {}

  /**
   * Get set of arg2 where R*(arg1, arg2) is true, given arg1.
   */
  unordered_set<T> getByFirstArgT(T arg1) const {
    auto result = this->table->get(arg1);
    for (auto r : result) {
      auto subResult = getByFirstArgT(r);
      result.insert(subResult.begin(), subResult.end());
    }
    return result;
  }

  /**
   * Get set of arg1 where R*(arg1, arg2) is true, given arg2.
   */
  unordered_set<T> getBySecondArgT(T arg2) const {
    auto result = this->reverseTable->get(arg2);
    for (auto r : result) {
      auto subResult = getBySecondArgT(r);
      result.insert(subResult.begin(), subResult.end());
    }
    return result;
  }

  /**
   * Find R*(arg1, arg2) where arg1 is in the given arg1Values and arg2
   * satisfies arg2Predicate.
   */
  QueryResult<T, T> queryT(unordered_set<T> arg1Values,
                           Predicate<T> arg2Predicate) const {
    QueryResult<T, T> result;
    for (auto arg1 : arg1Values) {
      auto arg2Values = getByFirstArgT(arg1);
      for (auto arg2 : arg2Values) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }
    return result;
  }

  /**
   * Find R*(arg1, arg2) where arg2 is in the given arg2Values and arg1
   * satisfies arg1Predicate.
   */
  QueryResult<T, T> queryT(Predicate<T> arg1Predicate,
                           unordered_set<T> arg2Values) const {
    QueryResult<T, T> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = getBySecondArgT(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return result;
  }

  /**
   * Find R*(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  QueryResult<T, T> queryT(T arg1, Predicate<T> arg2Predicate) const {
    return queryT(unordered_set<T>({arg1}), arg2Predicate);
  }

  /**
   * Find R*(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  QueryResult<T, T> queryT(Predicate<T> arg1Predicate, T arg2) const {
    return queryT(arg1Predicate, unordered_set<T>({arg2}));
  }
};
