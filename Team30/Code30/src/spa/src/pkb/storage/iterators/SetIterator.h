#pragma once

#include <set>

#include "IBaseIterator.h"

using std::set;

template <typename T>
class SetIterator : public IBaseIterator<T> {
 public:
  SetIterator(const set<T>* set) : set_(set), iter_(set->begin()) {}

  // Return the next item in the set
  T getNext() {
    if (iter_ == set_->end()) {
      // End of the set has been reached
      return T();
    }
    T item = *iter_;
    ++iter_;
    return item;
  }

 private:
  const set<T>* set_;
  typename set<T>::const_iterator iter_;
};
