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
 * Table manager for relation, R(leftArg, rightArg), where args are type K and V
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

  void insert(StmtValue leftArg, StmtValue rightArg) override;

  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue leftArg, StmtValue rightArg,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  unique_ptr<IBaseIterator<StmtValue>> getRightValIter(
      StmtValue leftArg) const override;

  unique_ptr<IBaseIterator<StmtValue>> getLeftValIter(
      StmtValue rightArg) const override;

  QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const StmtValueSet &arg1Values,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const StmtValueSet &arg2Values,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;
};
