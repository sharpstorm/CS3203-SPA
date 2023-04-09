/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

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
  unordered_set<Value> *allValues;
  EntityIdx index;

 public:
  IndexTableManager(IBaseTable<EntityIdx, Value> *table,
                    IBaseTable<Value, EntityIdx> *reverseTable,
                    unordered_set<Value> *values)
      : table(table), reverseTable(reverseTable), allValues(values), index(0) {}

  EntityIdx insert(Value value) {
    // check if already exists
    if (allValues->find(value) != allValues->end()) {
      return reverseTable->get(value);
    }
    index++;
    table->insert(index, value);
    reverseTable->insert(value, index);
    allValues->insert(value);
    return index;
  }

  Value getValueByIdx(const EntityIdx &idx) { return table->get(idx); }

  EntityIdx getIdxOfValue(const Value &value) {
    return reverseTable->get(value);
  }

  const unordered_set<Value> &getAllValues() { return *allValues; }
};
