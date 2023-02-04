#include "HashKeyTable.h"

template <typename K, typename V>
HashKeyTable<K, V>::HashKeyTable() : table() {}

template <typename K, typename V>
void HashKeyTable<K, V>::set(K key, V value) {
  table[key].insert(value);
}

template <typename K, typename V>
std::unordered_set<V> HashKeyTable<K, V>::get(K key) {
  return table[key];
}
