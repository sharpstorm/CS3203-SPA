#pragma once

#include <unordered_set>

#include "common/Types.h"
#include "pkb/storage/tables/HashKeyTable.h"
#include "pkb/storage/tables/IBaseTable.h"

using std::unordered_set;

template <typename Value>
class IndexTableManager {
 private:
  IBaseTable<EntityIdx, Value> *table;
  IBaseTable<Value, EntityIdx> *reverseTable;
  unordered_set<Value> allValues;
  EntityIdx index;

 public:
  IndexTableManager(IBaseTable<EntityIdx, Value> *table,
                    IBaseTable<Value, EntityIdx> *reverseTable)
      : table(table), reverseTable(reverseTable), index(0) {}

  EntityIdx insert(Value value) {
    // check if already exists
    if (allValues.find(value) != allValues.end()) {
      return reverseTable->get(value);
    }
    index++;
    table->set(index, value);
    reverseTable->set(value, index);
    allValues.insert(value);
    return index;
  }

  Value getValueByIdx(const EntityIdx &idx) { return table->get(idx); }

  EntityIdx getIdxOfValue(const Value &value) {
    return reverseTable->get(value);
  }

  unordered_set<Value> getAllValues() { return allValues; }
};
