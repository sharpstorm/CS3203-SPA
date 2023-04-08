#pragma once

#include <unordered_set>

using std::unordered_set;

template <typename V, typename T>
class IProvider {
 public:
  virtual ~IProvider() {}
  virtual const unordered_set<V>& getValuesOfType(T type) const = 0;
  virtual bool isValueOfType(T type, V value) const = 0;
};
