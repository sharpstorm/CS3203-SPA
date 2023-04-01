#pragma once
#include "pkb/PkbTypes.h"

/**
 * Table that maps item of type K to an item of type V.
 */
template <typename K, typename V>
class IBaseTable {
 public:
  virtual ~IBaseTable() {}
  virtual void insert(K key, V value) = 0;
  virtual const V& get(K key) const = 0;
  virtual void forEach(pkb::Function<K, V> function,
                       pkb::Terminator<K, V> terminator) const = 0;
  virtual int size() const = 0;
};
