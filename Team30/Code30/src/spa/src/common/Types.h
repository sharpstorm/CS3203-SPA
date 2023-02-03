#pragma once

#include <string>
#include <unordered_set>
#include <utility>

enum class EntityType { Statement, Variable, Constant, None };

enum class StmtType { Read, Print, Assign, Call, While, If, None };

struct StmtRef {
  StmtType type;
  int lineNum;
};

struct EntityRef {
  EntityType type;
  std::string name;
};

template <class T1, class T2>
struct std::hash<std::pair<T1, T2>> {
  std::size_t operator()(const std::pair<T1, T2>& pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

// note: error will occur if T or U is not primitive
template <typename T, typename U>
struct QueryResult {
  std::unordered_set<T> firstArgVals;
  std::unordered_set<U> secondArgVals;
  std::unordered_set<std::pair<T, U>> pairVals;
  bool isEmpty = true;

  void add(T first, U second) {
    firstArgVals.insert(first);
    secondArgVals.insert(second);
    pairVals.insert(std::make_pair(first, second));
    isEmpty = false;
  }
};
