#pragma once

template <typename T>
class Predicate {
 public:
  virtual bool operator()(T x) const = 0;
};
