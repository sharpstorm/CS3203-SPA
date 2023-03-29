#pragma once

/**
 * Table that maps item of type K to an item of type V.
 */
template <typename K, typename V>
class IBaseTable {
 public:
  virtual ~IBaseTable() {}
  virtual void insert(K key, V value) = 0;
  virtual const V& get(K key) const = 0;
};
