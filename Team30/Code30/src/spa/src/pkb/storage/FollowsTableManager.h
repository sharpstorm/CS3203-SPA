#pragma once

#include <memory>
#include <unordered_set>

#include "RelationTableManager.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/tables/IntSetTable.h"

using pkb::Predicate;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

/**
 * Table manager for relation, R(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> Set<StmtValue> and V-> Set<StmtValue>.
 * Provides insert and query functionalities.
 */

class FollowsTableManager : public RelationTableManager<StmtValue, StmtValue> {
 public:
  FollowsTableManager(IntSetTable<StmtValue> *table,
                      IntSetTable<StmtValue> *reverseTable)
      : RelationTableManager<StmtValue, StmtValue>(table, reverseTable) {}
  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  set<StmtValue> getByFirstArg(StmtValue arg1) const override {
    auto values = table->get(arg1);
    if (values.empty()) {
      return {};
    } else {
      return {*values.begin()};
    }
  }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  set<StmtValue> getBySecondArg(StmtValue arg2) const override {
    auto values = reverseTable->get(arg2);
    if (values.empty()) {
      return {};
    } else {
      return {*values.rbegin()};
    }
  }
};
