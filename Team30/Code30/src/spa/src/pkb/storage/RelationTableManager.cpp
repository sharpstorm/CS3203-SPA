#include "RelationTableManager.h"

template <typename K, typename V>
RelationTableManager<K, V>::RelationTableManager(BaseTable<K, V>* table,
                                                 BaseTable<V, K>* reverseTable)
    : table(table), reverseTable(reverseTable) {}

template <typename K, typename V>
void RelationTableManager<K, V>::insert(K arg1, V arg2) {
  table->set(arg1, arg2);
  reverseTable->set(arg2, arg1);
}

template <typename K, typename V>
unordered_set<V> RelationTableManager<K, V>::getByArg1(K arg1) {
  return table->get(arg1);
}

template <typename K, typename V>
unordered_set<K> RelationTableManager<K, V>::getByArg2(V arg2) {
  return reverseTable->get(arg2);
}

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
