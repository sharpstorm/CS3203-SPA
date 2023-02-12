#pragma once

/**
 * Table that maps item of type K to an item of type V.
 */
template<typename K, typename V>
class IBaseTable {
 public:
  virtual ~IBaseTable() {}
  virtual void set(K key, V value) = 0;
  virtual V get(K key) const = 0;
};