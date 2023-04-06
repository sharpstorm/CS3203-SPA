#pragma once

#include <vector>
#include <stack>

using std::vector, std::stack;

template<class T>
using VectorStack = stack<T, vector<T>>;
