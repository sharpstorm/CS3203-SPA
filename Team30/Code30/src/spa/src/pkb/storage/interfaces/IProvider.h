#pragma once

#include <unordered_set>

using std::unordered_set;

template <typename V, typename T>
class IProvider {
 public:
  virtual ~IProvider() {}
  unordered_set<V> getValuesOfType(T type);
  bool isValueOfType(V value, T type);
};
