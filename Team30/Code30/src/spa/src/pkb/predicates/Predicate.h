#pragma once

#include <functional>

template <typename T>
class RelPredicate {
 public:
  virtual bool operator()(T const &x) const = 0;
};

template <typename T>
using Predicate = std::function<bool(T const &)>;
