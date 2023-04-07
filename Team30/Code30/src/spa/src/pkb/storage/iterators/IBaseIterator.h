#pragma once

template <typename T>
class IBaseIterator {
 public:
  virtual ~IBaseIterator() {}
  virtual T getNext() = 0;
};
