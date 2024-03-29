/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once

#include <map>
#include <utility>

#include "IBaseTableIterator.h"

using std::map, std::pair;

template <typename K, typename V>
class MapIterator : public IBaseTableIterator<K, V> {
 public:
  explicit MapIterator(const map<K, V>& map) : map_(map), iter_(map.begin()) {}

  // Return the next key-value pair in the map
  pair<K, V> getNext() override {
    if (iter_ == map_.end()) {
      // End of the map has been reached
      return make_pair(K(), V());
    }
    pair<K, V> item = *iter_;
    ++iter_;
    return item;
  }

 private:
  const map<K, V>& map_;
  typename map<K, V>::const_iterator iter_;
};
