#pragma once

#include <memory>
#include <unordered_set>

#include "common/Types.h"
#include "pkb/PkbTypes.h"

using std::shared_ptr, std::unordered_set;

#include "tables/HashKeySetTable.h"

template<typename K, typename V>
class PatternTableManager {
 private:
  shared_ptr<HashKeySetTable<V, K>> reverseTable;

 public:
  explicit PatternTableManager(shared_ptr<HashKeySetTable<V, K>> reverseTable)
      : reverseTable(reverseTable) {}

  void insert(K arg1, V arg2) {
    reverseTable->set(arg2, arg1);
  }

  QueryResult<K, V> queryByValue(V arg2) const {
    QueryResult<K, V> result;
    auto arg1Values = reverseTable->get(arg2);
    for (auto arg1 : arg1Values) {
      result.add(arg1, arg2);
    }
    return result;
  }

  QueryResult<K, V> queryAll() const {
    QueryResult<K, V> result;
    for (auto itr = reverseTable->begin(); itr != reverseTable->end(); ++itr) {
      auto arg2 = itr->first;
      auto arg1Values = itr->second;
      for (auto arg1 : arg1Values) {
        result.add(arg1, arg2);
      }
    }
    return result;
  }
};
