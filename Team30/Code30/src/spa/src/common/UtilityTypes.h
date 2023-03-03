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

template<class T>
IntersectSetPtr<T> intersectSet(IntersectSet<T>* s1,
                                IntersectSet<T>* s2);
