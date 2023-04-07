#pragma once

#include <unordered_set>

#include "IBaseIterator.h"

using std::unordered_set;

template <typename T>
class SetIterator : public IBaseIterator<T> {
 public:
  explicit SetIterator(const unordered_set<T>& set)
      : set_(set), iter_(set.begin()) {}

  // Return the next item in the set
  T getNext() {
    if (iter_ == set_.end()) {
      // End of the set has been reached
      return T();
    }
    T item = *iter_;
    ++iter_;
    return item;
  }

 private:
  const unordered_set<T>& set_;
  typename unordered_set<T>::const_iterator iter_;
};
