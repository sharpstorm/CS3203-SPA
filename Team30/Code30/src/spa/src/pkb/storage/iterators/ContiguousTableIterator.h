#pragma once

#include <utility>
#include <vector>

#include "IBaseTableIterator.h"

using std::vector, std::pair;

template <typename V>
class ContiguousTableIterator : public IBaseTableIterator<int, V> {
 public:
  explicit ContiguousTableIterator(const vector<V>& vector)
      : vector_(vector), index_(1) {}

  // Return the next key-value pair in the vector
  pair<int, V> getNext() override {
    if (index_ >= vector_.size()) {  // End of the vector has been reached
      return make_pair(0, V());
    }
    V item = vector_[index_];
    pair<int, V> pairResult = make_pair(index_, item);
    index_++;
    return pairResult;
  }

 private:
  const vector<V>& vector_;
  int index_;
};
