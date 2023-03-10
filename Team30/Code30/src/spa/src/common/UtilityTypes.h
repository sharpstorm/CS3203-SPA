#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

using std::vector, std::unique_ptr, std::make_unique, std::unordered_set;

template<typename T>
using UniqueVectorPtr = unique_ptr<vector<T>>;

template<typename T>
using VectorPtr = vector<T>*;

template<class T>
using IntersectSet = unordered_set<T>;

template<class T>
using IntersectSetPtr = unique_ptr<IntersectSet<T>>;

template <class T>
struct AbstractIterator
{
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = T;
  using pointer = const T*;
  using reference = const T&;

  AbstractIterator(pointer ptr) : curPos(ptr) {}

  reference operator*() const {
    return *curPos;
  }

  pointer operator->() {
    return curPos;
  }

  AbstractIterator& operator++() {
    curPos++;
    return *this;
  }

  AbstractIterator operator++(int) {
    AbstractIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator== (const AbstractIterator& other) {
    return curPos == other.curPos;
  };

  bool operator!= (const AbstractIterator& other) {
    return curPos != other.curPos;
  };

 private:
  pointer curPos;
};
