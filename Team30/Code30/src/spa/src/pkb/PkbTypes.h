#pragma once

#include <functional>

template<typename T>
using Predicate = std::function<bool(T const &)>;

