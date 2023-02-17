#pragma once

#include <functional>

template<typename T>
using Predicate = std::function<bool(T const &)>;

template<typename T, typename R>
using Transformer = std::function<R(T const &)>;
