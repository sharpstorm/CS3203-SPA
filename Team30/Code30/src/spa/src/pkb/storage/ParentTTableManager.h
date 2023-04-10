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
  void insert(StmtValue leftArg, StmtValue rightArg) override;

  /**
   * Get set of rightArg where R(leftArg, rightArg) is true, given leftArg
   * value.
   */
  StmtValue getByFirstArg(StmtValue leftArg) const;

  StmtValue getLastSibling(StmtValue stmt) const;

  /**
   * Get set of leftArg where R(leftArg, rightArg) is true, given rightArg
   * value.
   */
  const StmtValueSet getBySecondArg(StmtValue rightArg) const;

  // const, const
  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue leftArg, StmtValue rightArg,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  // syn, const
  QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, const StmtValueSet &arg2Values,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  // const, syn
  QueryResultPtr<StmtValue, StmtValue> query(
      const StmtValueSet &arg1Values, Predicate<StmtValue> rightPredicate,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  /**
   * Return non-empty result if at least one such relation
   */
  QueryResultPtr<StmtValue, StmtValue> hasRelation(
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  /**
   * Right side wildcard.
   */
  QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const unordered_set<StmtValue> &leftArgValues,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;

  /**
   * Left side wildcard.
   */
  QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const unordered_set<StmtValue> &rightArgValues,
      QueryResultBuilder<StmtValue, StmtValue> *resultBuilder) const override;
};
