#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "IStorage.h"
#include "RelationTableManager.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/iterators/ItemIterator.h"
#include "pkb/storage/tables/ContiguousVectorTable.h"

using pkb::Predicate;
using std::make_unique, std::unique_ptr, std::vector, std::unordered_set;

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

  void insert(StmtValue arg1, StmtValue arg2) override {
    table->insert(arg1, arg2);
    reverseTable->insert(arg2, arg1);
  }

  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, StmtValue arg2,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    auto firstVal = table->getFirstValue(arg1);
    if (firstVal == arg2) {
      resultBuilder->add(arg1, arg2);
    }

    return resultBuilder->getResult();
  }

  unique_ptr<IBaseIterator<StmtValue>> getRightValIter(
      StmtValue leftArg) const override {
    auto rightArg = table->getFirstValue(leftArg);
    return make_unique<ItemIterator<StmtValue>>(rightArg);
  }

  unique_ptr<IBaseIterator<StmtValue>> getLeftValIter(
      StmtValue rightArg) const override {
    auto leftArg = reverseTable->getLastValue(rightArg);
    return make_unique<ItemIterator<StmtValue>>(leftArg);
  }

  QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const unordered_set<StmtValue> &arg1Values,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    for (auto arg1 : arg1Values) {
      auto arg2 = table->getFirstValue(arg1);
      if (arg2 != 0) {
        resultBuilder->addLeft(arg1);
      }
    }
    return resultBuilder->getResult();
  }

  QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const unordered_set<StmtValue> &arg2Values,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      auto arg1 = reverseTable->getLastValue(arg2);
      if (arg1 != 0) {
        resultBuilder->addRight(arg2);
      }
    }
    return resultBuilder->getResult();
  }
};
