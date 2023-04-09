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
  void insert(StmtValue arg1, StmtValue arg2) override;

  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  StmtValue getByFirstArg(StmtValue arg1) const;

  StmtValue getLastSibling(StmtValue stmt) const;

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  const StmtValueSet getBySecondArg(StmtValue arg2) const;

  // const, const
  QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, StmtValue arg2,
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
