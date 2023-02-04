#pragma once

#include <memory>
#include <unordered_set>

#include "../../common/Types.h"
#include "tables/BaseTable.h"

using std::shared_ptr;
using std::unordered_set;

template <typename K, typename V>
class RelationTableManager {
 private:
  BaseTable<K, V>* table;         // maps T -> set<U>
  BaseTable<V, K>* reverseTable;  // maps U -> set<T>

 public:
  RelationTableManager(BaseTable<K, V>* table, BaseTable<V, K>* reverseTable);

  void insert(K arg1, V arg2);

  unordered_set<V> getByArg1(K arg1);

  unordered_set<K> getByArg2(V arg2);
  // QueryResult<K, V>* query(unordered_set<T> key1Set,
  //                         Predicate<U> key2Predicate) {
  //  QueryResult<K, V>* result = new QueryResult<K, V>();

  //  for (key1 : key1Set) {
  //    U keys2 = table.get(key1);
  //    for (key2 : keys2) {
  //      if (key2Predicate.test(key2)) {
  //        result.add(key1, key2);
  //      }
  //    }
  //  }
  //  return result;
  //}
  // QueryResult<K, V>* query(T arg1, Predicate<U> arg2Predicate) {
  //  return query(unordered_set<T>({arg1}))
  //}

  // getByKey(T arg1)
  // getByValue(T arg1)
};
