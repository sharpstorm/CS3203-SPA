#pragma once

#include <set>

using std::set;

template <typename V, typename T>
class IProvider {
 public:
  virtual ~IProvider() {}
  virtual const set<V>& getValuesOfType(T type) const = 0;
  virtual bool isValueOfType(T type, V value) const = 0;
};
