#pragma once

#include <memory>
#include <vector>

#include "IStorage.h"
#include "RelationTableManager.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/iterators/ItemIterator.h"
#include "pkb/storage/tables/ContiguousVectorTable.h"

using pkb::Predicate;
using std::make_unique, std::unique_ptr, std::vector;

/**
 * Table manager for relation, R(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> Set<StmtValue> and V-> Set<StmtValue>.
 * Provides insert and query functionalities.
 */

class FollowsTableManager : public RelationTableManager<StmtValue, StmtValue> {
 protected:
  ContiguousVectorTable<StmtValue> *table;
  ContiguousVectorTable<StmtValue> *reverseTable;

 public:
  FollowsTableManager(ContiguousVectorTable<StmtValue> *table,
                      ContiguousVectorTable<StmtValue> *reverseTable)
      : table(table),
        reverseTable(reverseTable),
        RelationTableManager(table, reverseTable) {}

  void insert(StmtValue arg1, StmtValue arg2) {
    table->insert(arg1, arg2);
    reverseTable->insert(arg2, arg1);
  }

  QueryResultPtr<StmtValue, StmtValue> query(StmtValue arg1,
                                             StmtValue arg2) const override {
    QueryResult<StmtValue, StmtValue> result;
    auto firstVal = table->getFirstValue(arg1);
    if (firstVal == arg2) {
      result.add(arg1, arg2);
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  unique_ptr<IBaseIterator<StmtValue>> getRightValIter(
      StmtValue leftArg) const override {
    auto rightArg = table->getFirstValue(leftArg);
    return make_unique<ItemIterator<StmtValue>>(rightArg);
  }

  virtual unique_ptr<IBaseIterator<StmtValue>> getLeftValIter(
      StmtValue rightArg) const override {
    auto leftArg = reverseTable->getLastValue(rightArg);
    return make_unique<ItemIterator<StmtValue>>(leftArg);
  }

  QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const set<StmtValue> &arg1Values) const override {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      auto arg2 = table->getFirstValue(arg1);
      if (arg2 != 0) {
        result.firstArgVals.insert(arg1);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const set<StmtValue> &arg2Values) const override {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      auto arg1 = reverseTable->getLastValue(arg2);
      if (arg1 != 0) {
        result.secondArgVals.insert(arg2);
        result.isEmpty = false;
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }
};
