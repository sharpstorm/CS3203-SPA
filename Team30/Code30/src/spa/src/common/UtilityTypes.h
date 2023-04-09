#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <set>

using std::vector, std::unique_ptr, std::make_unique, std::unordered_set,
    std::set;

template<typename T>
using UniqueVectorPtr = unique_ptr<vector<T>>;

template<typename T>
using VectorPtr = vector<T> *;

template<class T>
using IntersectSet = unordered_set<T>;

template<class T>
class AbstractIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = T;
  using pointer = const T *;
  using reference = const T &;

  explicit AbstractIterator(pointer ptr) : curPos(ptr) {}

  reference operator*() const {
    return *curPos;
  }

  pointer operator->() {
    return curPos;
  }

  AbstractIterator &operator++() {
    curPos++;
    return *this;
  }

  AbstractIterator operator++(int) {
    AbstractIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const AbstractIterator &other) {
    return curPos == other.curPos;
  }

  bool operator!=(const AbstractIterator &other) {
    return curPos != other.curPos;
  }

 private:
  pointer curPos;
};
