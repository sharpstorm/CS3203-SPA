#pragma once

#include <memory>
#include <set>

#include "FollowsTableManager.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "tables/ContiguousVectorTable.h"

using pkb::Predicate;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

class FollowsTTableManager : public FollowsTableManager {
 public:
  using FollowsTableManager::FollowsTableManager;
  /**
   * Find R(arg1, arg2) where arg1 and arg2 are given values.
   */
  QueryResultPtr<StmtValue, StmtValue> query(StmtValue arg1,
                                             StmtValue arg2) const override {
    QueryResult<StmtValue, StmtValue> result;
    if (table->contains(arg1, arg2)) {
      result.add(arg1, arg2);
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  QueryResultPtr<StmtValue, StmtValue> query(
      set<StmtValue> arg1Values,
      Predicate<StmtValue> arg2Predicate) const override {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      auto it = table->getValueIterator(arg1);
      StmtValue arg2;
      while ((arg2 = it->getNext()) != 0) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, set<StmtValue> arg2Values) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = reverseTable->get(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Find R(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, Predicate<StmtValue> arg2Predicate) const {
    return query(set<StmtValue>({arg1}), arg2Predicate);
  }

  /**
   * Find R(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, StmtValue arg2) const {
    return query(arg1Predicate, set<StmtValue>({arg2}));
  }

  /**
   * Return non-empty result if at least one such relation
   */
  virtual QueryResultPtr<StmtValue, StmtValue> hasRelation() const {
    QueryResult<StmtValue, StmtValue> result;
    if (table->size() != 0) {
      result.isEmpty = false;
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Right side wildcard.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const set<StmtValue> &arg1Values) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      if (table->containsKey(arg1)) {
        result.firstArgVals.insert(arg1);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Left side wildcard.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const set<StmtValue> &arg2Values) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      if (reverseTable->containsKey(arg2)) {
        result.secondArgVals.insert(arg2);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }
};
