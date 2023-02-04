#pragma once

#include <unordered_set>

template <typename K, typename V>
class BaseTable {
 public:
  virtual void set(K key, V value) {}
  virtual std::unordered_set<V> get(K key) {}
};
