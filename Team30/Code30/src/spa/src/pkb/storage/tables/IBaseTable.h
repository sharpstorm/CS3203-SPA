#pragma once

/**
 * Table that maps item of type K to an item of type V.
 */
template<typename K, typename V>
class IBaseTable {
 public:
  virtual ~IBaseTable() {}
  virtual void insert(K key, V value) = 0;
  virtual const V& get(K key) const = 0;

  // todo: begin and end will be needed in future
  virtual void begin() const = 0;
  virtual void end() const = 0;
};
