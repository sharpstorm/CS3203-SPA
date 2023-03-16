#pragma once

#include <unordered_set>

using std::unordered_set;

template <typename V, typename T>
class IProvider {
 public:
  virtual ~IProvider() {}
  virtual unordered_set<V> getValuesOfType(T type) const = 0;
  virtual bool isValueOfType(V value, T type) const = 0;
};
