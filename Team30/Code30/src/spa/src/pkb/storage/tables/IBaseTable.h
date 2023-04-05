#pragma once

#include <memory>

#include "pkb/PkbTypes.h"
#include "pkb/storage/iterators/IBaseTableIterator.h"

using std::unique_ptr, std::make_unique;

/**
 * Table that maps item of type K to an item of type V.
 */
template <typename K, typename V>
class IBaseTable {
 public:
  virtual ~IBaseTable() {}
  virtual void insert(K key, V value) = 0;
  virtual const V& get(K key) const = 0;
  virtual int size() const = 0;
  virtual bool isEmpty() const = 0;
  virtual unique_ptr<IBaseTableIterator<K, V>> getTableIterator() = 0;
};
