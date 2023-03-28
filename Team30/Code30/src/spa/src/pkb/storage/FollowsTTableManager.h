#pragma once

#include <memory>
#include <unordered_set>

#include "FollowsTableManager.h"
#include "RelationTableManager.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/tables/IntSetTable.h"
#include "tables/IBaseSetTable.h"

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

class FollowsTTableManager : public FollowsTableManager {
 public:
  using FollowsTableManager::FollowsTableManager;

  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  set<StmtValue> getByFirstArg(StmtValue arg1) const override {
    return table->get(arg1);
  }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  set<StmtValue> getBySecondArg(StmtValue arg2) const override {
    return reverseTable->get(arg2);
  }
};
