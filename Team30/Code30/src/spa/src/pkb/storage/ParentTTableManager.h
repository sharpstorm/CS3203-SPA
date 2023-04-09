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

class ParentTTableManager : public IStorage<StmtValue, StmtValue> {
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
  void insert(StmtValue arg1, StmtValue arg2) override {
    // keep only maxChild
    if (table->get(arg1) < arg2) {
      table->insert(arg1, arg2);
    }
    reverseTable->insert(arg2, arg1);
  }

  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  StmtValue getByFirstArg(StmtValue arg1) const { return table->get(arg1); }

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
  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, StmtValue arg2,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    auto maxChild = getByFirstArg(arg1);
    if (arg1 < arg2 && arg2 <= maxChild) {
      resultBuilder->add(arg1, arg2);
    }
    return resultBuilder->getResult();
  }

  // syn, const
  QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, StmtValueSet arg2Values,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = getBySecondArg(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          resultBuilder->add(arg1, arg2);
        }
      }
    }
    return resultBuilder->getResult();
  }

  // const, syn
  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValueSet arg1Values, Predicate<StmtValue> rightPredicate,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    for (auto arg1 : arg1Values) {
      auto maxChild = getByFirstArg(arg1);
      if (maxChild == 0) {
        continue;
      }
      for (int i = arg1 + 1; i <= maxChild; i++) {
        if (rightPredicate(i)) {
          resultBuilder->add(arg1, i);
        }
      }
    }
    return resultBuilder->getResult();
  }

  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, Predicate<StmtValue> arg2Predicate,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    return query(StmtValueSet({arg1}), arg2Predicate, resultBuilder);
  }

  QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, StmtValue arg2,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    return query(arg1Predicate, StmtValueSet({arg2}), resultBuilder);
  }

  /**
   * Return non-empty result if at least one such relation
   */
  QueryResultPtr<StmtValue, StmtValue> hasRelation(
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    if (!table->isEmpty()) {
      resultBuilder->setIsNotEmpty();
    }
    return resultBuilder->getResult();
  }

  /**
   * Right side wildcard.
   */
  QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const unordered_set<StmtValue> &leftArgValues,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    for (auto leftArg : leftArgValues) {
      auto rightArg = getByFirstArg(leftArg);
      if (rightArg != 0) {
        resultBuilder->addLeft(leftArg);
      }
    }
    return resultBuilder->getResult();
  }

  /**
   * Left side wildcard.
   */
  QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const unordered_set<StmtValue> &rightArgValues,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto rightArg : rightArgValues) {
      auto leftArg = getBySecondArg(rightArg);
      if (!leftArg.empty()) {
        resultBuilder->addRight(rightArg);
      }
    }
    return resultBuilder->getResult();
  }
};
