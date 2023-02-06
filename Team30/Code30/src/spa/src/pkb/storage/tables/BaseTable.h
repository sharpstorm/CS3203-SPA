#pragma once

#include <unordered_set>

/**
 * Table that maps item of type K to a set of type V items.
 */
template <typename K, typename V>
class BaseTable {
 public:
  virtual ~BaseTable() {}
  virtual void set(K key, V value) = 0;
  virtual std::unordered_set<V> get(K key) const = 0;
};
