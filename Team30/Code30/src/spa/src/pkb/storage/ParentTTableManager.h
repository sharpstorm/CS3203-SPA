#pragma once

#include <memory>
#include <unordered_set>

#include "FollowsTableManager.h"
#include "RelationTableManager.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/interfaces/IProvider.h"
#include "pkb/storage/tables/IntSetTable.h"
#include "tables/IBaseSetTable.h"

using pkb::Predicate;
using std::make_unique, std::unique_ptr, std::unordered_set;

class ParentTTableManager {
 private:
  // parent* -> max_child
  IntTable<StmtValue> *table;
  // max_child of block -> parents*
  IntSetTable<StmtValue> *reverseTable;
  ContiguousVectorTable<StmtValue> *followsTable;

 public:
  ParentTTableManager(IntTable<StmtValue> *table,
                      IntSetTable<StmtValue> *reverseTable,
                      ContiguousVectorTable<StmtValue> *followsTable)
      : table(table), reverseTable(reverseTable), followsTable(followsTable) {}
  void insert(StmtValue arg1, StmtValue arg2) {
    // keep only maxChild
    if (table->get(arg1) < arg2) {
      table->insert(arg1, arg2);
    }
    reverseTable->insert(arg2, arg1);
  }

  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  const StmtValue getByFirstArg(StmtValue arg1) const {
    return table->get(arg1);
  }

  StmtValue getLastSibling(StmtValue stmt) const {
    const StmtValue lastVal = followsTable->getLastValue(stmt);
    if (lastVal == 0) {
      return stmt;
    } else {
      return lastVal;
    }
  }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  const StmtValueSet getBySecondArg(StmtValue arg2) const {
    // find max sibling
    auto maxSibling = getLastSibling(arg2);
    return reverseTable->get(maxSibling);
  }

  // const, const
  QueryResultPtr<StmtValue, StmtValue> query(StmtValue arg1,
                                             StmtValue arg2) const {
    QueryResult<StmtValue, StmtValue> result;
    auto maxChild = getByFirstArg(arg1);
    if (arg1 < arg2 && arg2 <= maxChild) {
      result.add(arg1, arg2);
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  // syn, const
  QueryResultPtr<StmtValue, StmtValue> query(Predicate<StmtValue> arg1Predicate,
                                             StmtValueSet arg2Values) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = getBySecondArg(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  // const, syn
  QueryResultPtr<StmtValue, StmtValue> query(
      const StmtValueSet &arg1Values,
      Predicate<StmtValue> rightPredicate)
      const {  // change to unordered_set<StmtValue>&
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      auto maxChild = getByFirstArg(arg1);
      if (maxChild == 0) {
        continue;
      }
      for (int i = arg1 + 1; i <= maxChild; i++) {
        if (rightPredicate(i)) {
          result.add(arg1, i);
        }
      }
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, StmtValue arg2) const {
    return query(arg1Predicate, StmtValueSet({arg2}));
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
      const unordered_set<StmtValue> &leftArgValues) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto leftArg : leftArgValues) {
      auto rightArg = getByFirstArg(leftArg);
      if (rightArg != 0) {
        result.firstArgVals.insert(leftArg);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Left side wildcard.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const unordered_set<StmtValue> &rightArgValues) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto rightArg : rightArgValues) {
      auto leftArg = getBySecondArg(rightArg);
      if (leftArg.size() > 0) {
        result.secondArgVals.insert(rightArg);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }
};
