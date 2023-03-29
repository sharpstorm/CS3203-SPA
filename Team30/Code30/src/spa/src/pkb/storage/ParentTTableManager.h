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
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

class ParentTTableManager {
 private:
  // parent -> max_child
  IntTable<StmtValue> *table;
  // max_child -> parents*
  IntSetTable<StmtValue> *reverseTable;
  IntSetTable<StmtValue> *followsTable;

 public:
  ParentTTableManager(IntTable<StmtValue> *table,
                      IntSetTable<StmtValue> *reverseTable,
                      IntSetTable<StmtValue> *followsTable)
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
  const set<StmtValue> getByFirstArg(StmtValue arg1) const {
    return {table->get(arg1)};
  }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  const set<StmtValue> getBySecondArg(StmtValue arg2) const {
    // find max sibling
    auto maxSibling = followsTable->getFirstValue(arg2);
    if (maxSibling == 0) {
      maxSibling = arg2;
    }
    return reverseTable->get(maxSibling);
  }

  // const, const
  QueryResultPtr<StmtValue, StmtValue> query(StmtValue arg1,
                                             StmtValue arg2) const {
    QueryResult<StmtValue, StmtValue> result;
    auto maxChild =
        *(getByFirstArg(arg1).begin());  // guaranteed not to be empty
    if (arg1 < arg2 && arg2 <= maxChild) {
      result.add(arg1, arg2);
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  // syn, const
  QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate,
      unordered_set<StmtValue> arg2Values) const {
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
      unordered_set<StmtValue> arg1Values,
      set<StmtValue> &rightValues) const {  // change to set<StmtValue>&
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      auto maxChild = *(getByFirstArg(arg1).begin());
      auto itLower = rightValues.lower_bound(arg1 + 1);
      auto itUpper = rightValues.upper_bound(maxChild);

      for (auto it = itLower; it != itUpper; it++) {
        result.add(arg1, *it);
      }
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, StmtValue arg2) const {
    return query(arg1Predicate, unordered_set<StmtValue>({arg2}));
  }
};
