#pragma once

#include <vector>

#include "IBaseIterator.h"

using std::vector;

template <typename T>
class ItemIterator : public IBaseIterator<T> {
 public:
  explicit ItemIterator(T item) : item(item) {}

  // Return the next element in the vector
  T getNext() {
    T result = item;
    item = T();
    return result;
  }

 private:
  T item;
};
