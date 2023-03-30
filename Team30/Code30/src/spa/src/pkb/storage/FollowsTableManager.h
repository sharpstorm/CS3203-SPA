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

  /**
   * Query by iterating rows of table.
   */
  //  virtual QueryResultPtr<StmtValue, StmtValue> query(
  //      Predicate<StmtValue> rightPredicate) const {
  //    QueryResult<StmtValue, StmtValue> result;
  //
  //    for (auto itr = table->begin(); itr != table->end(); ++itr) {
  //      auto leftVal = itr->first;
  //      auto rightValues = itr->second;
  //      if (rightValues.empty()) {
  //        continue;
  //      }
  //      auto firstVal = *(rightValues.begin());
  //      if (rightPredicate(firstVal)) {
  //        result.add(leftVal, firstVal);
  //      }
  //    }
  //    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  //  }
};
