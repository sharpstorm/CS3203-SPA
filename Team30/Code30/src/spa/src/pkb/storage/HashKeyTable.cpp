#include "HashKeyTable.h"

HashKeyTable::HashKeyTable() : table() {}

void HashKeyTable::set(K key, V value) { table[key].insert(value); }

std::unordered_set<V> HashKeyTable::get(K key) { return table[key]; }
