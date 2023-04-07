#pragma once

#include <memory>
#include <set>

#include "pkb/PkbTypes.h"
#include "pkb/storage/iterators/IBaseIterator.h"
#include "pkb/storage/iterators/IBaseTableIterator.h"

using std::set, std::unique_ptr;
/**
 * Table that maps item of type K to a set of type V items.
 */
template <typename K, typename V>
class IBaseSetTable {
 public:
  virtual ~IBaseSetTable() {}
  virtual void insert(K key, V value) = 0;
  virtual int size() const = 0;
  virtual bool isEmpty() const = 0;
  virtual bool contains(K key, V value) const = 0;
  virtual bool containsKey(K key) const = 0;
  virtual unique_ptr<IBaseIterator<V>> getValueIterator(K key) = 0;
};
