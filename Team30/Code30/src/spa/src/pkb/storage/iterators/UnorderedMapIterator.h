#pragma once

#include <unordered_map>

#include "IBaseTableIterator.h"

using std::unordered_map;

template <typename K, typename V>
class UnorderedMapIterator : public IBaseTableIterator<K, V> {
 public:
  UnorderedMapIterator(const unordered_map<K, V>& map)
      : map_(map), iter_(map.begin()) {}

  // Return the next key-value pair in the unordered_map
  pair<K, V> getNext() {
    if (iter_ == map_.end()) {
      // End of the unordered_map has been reached
      return make_pair(K(), V());
    }
    pair<K, V> item = *iter_;
    ++iter_;
    return item;
  }

 private:
  const unordered_map<K, V>& map_;
  typename unordered_map<K, V>::const_iterator iter_;
};
