#pragma once

#include <memory>
#include <unordered_set>

#include "../../common/Types.h"
#include "../predicates/Predicate.h"
#include "tables/BaseTable.h"

using std::shared_ptr;
using std::unordered_set;

template <typename K, typename V>
class RelationTableManager {
 private:
  shared_ptr<BaseTable<K, V>> table;         // maps K -> set<V>
  shared_ptr<BaseTable<V, K>> reverseTable;  // maps V -> set<K>

 public:
  RelationTableManager(shared_ptr<BaseTable<K, V>> table,
                       shared_ptr<BaseTable<V, K>> reverseTable)
      : table(table), reverseTable(reverseTable) {}

  void insert(K arg1, V arg2) {
    table->set(arg1, arg2);
    reverseTable->set(arg2, arg1);
  }

  unordered_set<V> getByFirstArg(K arg1) { return table->get(arg1); }

  unordered_set<K> getBySecondArg(V arg2) { return reverseTable->get(arg2); }

  QueryResult<K, V> query(unordered_set<K> arg1Values,
                          Predicate<V> arg2Predicate) {
    QueryResult<K, V> result;
    for (auto arg1 : arg1Values) {
      auto arg2Values = table->get(arg1);
      for (auto arg2 : arg2Values) {
        if (arg2Predicate(arg2)) {
          result.add(arg1, arg2);
        }
      }
    }
    return result;
  }

  QueryResult<K, V> query(Predicate<K> arg1Predicate,
                          unordered_set<V> arg2Values) {
    QueryResult<K, V> result;
    for (auto arg2 : arg2Values) {
      auto arg1Values = reverseTable->get(arg2);
      for (auto arg1 : arg1Values) {
        if (arg1Predicate(arg1)) {
          result.add(arg1, arg2);
        }
      }
    }
    return result;
  }

  QueryResult<K, V> query(K arg1, Predicate<V> arg2Predicate) {
    return query(unordered_set<K>({arg1}), arg2Predicate);
  }

  QueryResult<K, V> query(Predicate<K> arg1Predicate, V arg2) {
    return query(arg1Predicate, unordered_set<V>({arg2}));
  }
};
