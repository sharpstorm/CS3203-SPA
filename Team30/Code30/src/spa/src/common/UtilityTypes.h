#pragma once

#include <vector>
#include <memory>

using std::vector, std::unique_ptr, std::make_unique;

template<typename T>
using UniqueVectorPtr = unique_ptr<vector<T>>;

template<typename T>
using VectorPtr = vector<T>*;
