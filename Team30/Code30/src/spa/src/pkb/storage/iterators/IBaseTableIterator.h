#pragma once

#include <utility>

using std::pair, std::make_pair;

template <typename K, typename V>
class IBaseTableIterator {
 public:
  virtual ~IBaseTableIterator(){};
  virtual pair<K, V> getNext() = 0;
};
