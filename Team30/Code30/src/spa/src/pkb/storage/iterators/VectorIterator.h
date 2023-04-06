#pragma once

#include <vector>

#include "IBaseIterator.h"

using std::vector;

template <typename T>
class VectorIterator : public IBaseIterator<T> {
 public:
  explicit VectorIterator(const vector<T>& vector)
      : vector_(vector), it_(vector.begin()) {}

  // Return the next element in the vector
  T getNext() {
    if (it_ == vector_.end()) {
      // End of the vector has been reached
      return T();
    }
    T item = *it_;
    ++it_;
    return item;
  }

 private:
  const vector<T>& vector_;
  typename vector<T>::const_iterator it_;
};
