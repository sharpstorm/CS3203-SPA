#pragma once

#include <memory>

#include "IStorage.h"
#include "common/Types.h"
#include "pkb/PkbTypes.h"
#include "pkb/storage/tables/ContiguousVectorTable.h"

using pkb::Predicate;
using std::make_unique, std::unique_ptr;

/**
 * Table manager for relation, R(arg1, arg2), where args are type K and V
 * respectively. Stores mapping of K -> Set<StmtValue> and V-> Set<StmtValue>.
 * Provides insert and query functionalities.
 */

class FollowsTableManager : public IStorage<StmtValue, StmtValue> {
 protected:
  ContiguousVectorTable<StmtValue> *table;  // maps StmtValue -> set<StmtValue>
  ContiguousVectorTable<StmtValue>
      *reverseTable;  // maps StmtValue -> set<StmtValue>

 public:
  FollowsTableManager(ContiguousVectorTable<StmtValue> *table,
                      ContiguousVectorTable<StmtValue> *reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(StmtValue arg1, StmtValue arg2) {
    table->insert(arg1, arg2);
    reverseTable->insert(arg2, arg1);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(StmtValue arg1,
                                                     StmtValue arg2) const {
    QueryResult<StmtValue, StmtValue> result;
    auto firstVal = table->getFirstValue(arg1);
    if (firstVal == arg2) {
      result.add(arg1, arg2);
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(
      set<StmtValue> arg1Values, Predicate<StmtValue> arg2Predicate) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg1 : arg1Values) {
      auto arg2 = table->getFirstValue(arg1);
      if (arg2 != 0 && arg2Predicate(arg2)) {
        result.add(arg1, arg2);
      }
    }

    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, set<StmtValue> arg2Values) const {
    QueryResult<StmtValue, StmtValue> result;
    for (auto arg2 : arg2Values) {
      auto arg1 = reverseTable->getLastValue(arg2);
      if (arg1 != 0 && arg1Predicate(arg1)) {
        result.add(arg1, arg2);
      }
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(
      StmtValue arg1, Predicate<StmtValue> arg2Predicate) const {
    return query(set<StmtValue>({arg1}), arg2Predicate);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> query(
      Predicate<StmtValue> arg1Predicate, StmtValue arg2) const {
    return query(arg1Predicate, set<StmtValue>({arg2}));
  }

  virtual QueryResultPtr<StmtValue, StmtValue> hasRelation() const {
    QueryResult<StmtValue, StmtValue> result;
    if (table->size() != 0) {
      result.isEmpty = false;
    }
    return make_unique<QueryResult<StmtValue, StmtValue>>(result);
  }

  virtual QueryResultPtr<StmtValue, StmtValue> rightWildcardQuery(
      const set<StmtValue> &arg1Values) const {
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

  virtual QueryResultPtr<StmtValue, StmtValue> leftWildcardQuery(
      const set<StmtValue> &arg2Values) const {
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
