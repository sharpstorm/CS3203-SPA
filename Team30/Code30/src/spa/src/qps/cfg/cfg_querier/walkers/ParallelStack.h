#pragma once

#include <vector>
using std::vector;

template<class T, class U>
class ParallelStack {
 private:
  vector<T> left;
  vector<U> right;

 public:
  void push(const T &leftItem, const U &rightItem) {
    left.push_back(leftItem);
    right.push_back(rightItem);
  }

  T getLeftBack() {
    return left.back();
  }

  U getRightBack() {
    return right.back();
  }

  void pop() {
    left.pop_back();
    right.pop_back();
  }

  bool empty() {
    return left.empty() && right.empty();
  }
};
