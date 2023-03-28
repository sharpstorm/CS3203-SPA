#pragma once
#include <memory>
#include <set>
#include "RelationTableManager.h"
using pkb::Predicate;
using std::make_unique;
using std::unique_ptr;

/**
 * Table manager for transitive relations, R*(arg1, arg2) where args are type T.
 * R*(arg1, arg2) is true if
 * - R(arg1, arg2) or
 * - R(arg1, a) and R*(a, arg2) for some a
 */
template <typename T>
class TransitiveRelationTableManager : public RelationTableManager<T, T> {
 public:
  TransitiveRelationTableManager(IBaseSetTable<T, T> *table,
                                 IBaseSetTable<T, T> *reverseTable)
      : RelationTableManager<T, T>(table, reverseTable) {}

  /**
   * Get set of arg2 where R*(arg1, arg2) is true, given arg1.
   */
  set<T> getByFirstArg(T arg1) const override {
    auto result = set<T>({});
    getByFirstArgTHelper(arg1, &result);
    return result;
  }

  /**
   * Get set of arg1 where R*(arg1, arg2) is true, given arg2.
   */
  set<T> getBySecondArg(T arg2) const override {
    auto result = set<T>({});
    getBySecondArgTHelper(arg2, &result);
    return result;
  }

  /**s
   * Find R*(arg1, arg2) where arg1 is in the given arg1Values and arg2
   * satisfies arg2Predicate.
   */
  QueryResultPtr<T, T> query(
      set<T> arg1Values, Predicate<T> arg2Predicate) const override {
    QueryResult<T, T> result;
    for (auto arg1 : arg1Values) {
      auto arg2Values = getByFirstArg(arg1);
      for (auto arg2 : arg2Values) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }
    return make_unique<QueryResult<T, T>>(result);
  }

  /**
   * Find R*(arg1, arg2) where arg2 is in the given arg2Values and arg1
   * satisfies arg1Predicate.
   */
  QueryResultPtr<T, T> query(
      Predicate<T> arg1Predicate, set<T> arg2Values) const override {
    QueryResult<T, T> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = getBySecondArg(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return make_unique<QueryResult<T, T>>(result);
  }

  /**
   * Find R*(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  QueryResultPtr<T, T> query(
      T arg1, Predicate<T> arg2Predicate) const override {
    return query(set<T>({arg1}), arg2Predicate);
  }

  /**
   * Find R*(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  QueryResultPtr<T, T> query(Predicate<T> arg1Predicate,
                                      T arg2) const override {
    return query(arg1Predicate, set<T>({arg2}));
  }

 private:
  void getByFirstArgTHelper(T arg1, set<T> *allResults) const {
    auto result = this->table->get(arg1);
    for (auto r : result) {
      if (allResults->find(r) != allResults->end()) {
        continue;
      }
      allResults->insert(r);
      getByFirstArgTHelper(r, allResults);
    }
    return;
  }

  void getBySecondArgTHelper(T arg2, set<T> *allResults) const {
    auto result = this->reverseTable->get(arg2);
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
