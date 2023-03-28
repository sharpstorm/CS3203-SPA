#pragma once

#include <set>

using std::set;
/**
 * Table that maps item of type K to a set of type V items.
 */
template<typename K, typename V>
class IBaseSetTable {
 public:
  virtual ~IBaseSetTable() {}
  virtual void insert(K key, V value) = 0;
  virtual const set<V>& get(K key) const = 0;

  // todo: begin and end will be needed in future
  virtual void begin() const = 0;
  virtual void end() const = 0;
};
