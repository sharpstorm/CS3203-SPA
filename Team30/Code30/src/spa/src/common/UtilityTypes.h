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
