#pragma once

#include <memory>
#include <unordered_set>

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

class FollowsTableManager {
 protected:
  IntSetTable<StmtValue> *table;
  IntSetTable<StmtValue> *reverseTable;

 public:
  FollowsTableManager(IntSetTable<StmtValue> *table,
                      IntSetTable<StmtValue> *reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(StmtValue arg1, StmtValue arg2) {
    table->insert(arg1, arg2);
    reverseTable->insert(arg2, arg1);
  }

  /**
   * Get set of arg2 where R(arg1, arg2) is true, given arg1 value.
   */
  virtual set<StmtValue> getByFirstArg(StmtValue arg1) const {
    auto value = table->getFirstValue(arg1);
    if (value == 0) {
      return {};
    } else {
      return { value };
    }
  }

  /**
   * Get set of arg1 where R(arg1, arg2) is true, given arg2 value.
   */
  virtual set<StmtValue> getBySecondArg(StmtValue arg2) const {
    auto value = reverseTable->getLastValue(arg2);
    if (value == 0) {
      return {};
    } else {
      return { value };
    }
  }

  /**
   * Find R(arg1, arg2) where arg1 is in the given arg1Values and arg2 satisfies
   * arg2Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      unordered_set<StmtValue> arg1Values,
      Predicate<StmtValue> arg2Predicate) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      auto arg2Values = getByFirstArg(arg1);
      for (auto arg2 : arg2Values) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  /**
   * Find R(arg1, arg2) where arg2 is in the given arg2Values and arg1 satisfies
   * arg1Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
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

  /**
   * Find R(arg1, arg2) given arg1 and arg2 satisfies arg2Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, Predicate<StmtValue> arg2Predicate) const {
    return query(unordered_set<StmtValue>({arg1}), arg2Predicate);
  }

  /**
   * Find R(arg1, arg2) given arg2 and arg1 satisfies arg1Predicate.
   */
  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> leftPredicate, StmtValue rightVal) const {
    return query(leftPredicate, unordered_set<StmtValue>({rightVal}));
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
