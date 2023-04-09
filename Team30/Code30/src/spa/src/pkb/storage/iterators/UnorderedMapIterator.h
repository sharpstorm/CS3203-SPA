/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include <unordered_map>
#include <utility>

#include "IBaseTableIterator.h"

using std::unordered_map, std::pair;

template <typename K, typename V>
class UnorderedMapIterator : public IBaseTableIterator<K, V> {
 public:
  explicit UnorderedMapIterator(const unordered_map<K, V>& map)
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
