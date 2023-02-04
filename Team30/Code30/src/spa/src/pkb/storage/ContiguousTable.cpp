#include "ContiguousTable.h"

ContiguousTable::ContiguousTable() : table() {}

void ContiguousTable::set(int key, V value) { table[key].insert(value); }

std::unordered_set<V> ContiguousTable::get(int key) { return table[key]; }
