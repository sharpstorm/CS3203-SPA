#pragma once

#include <string>
#include <unordered_set>
#include <utility>

using std::pair;
using std::unordered_set;

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
struct std::hash<pair<T1, T2>> {
  std::size_t operator()(const pair<T1, T2>& pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

template <typename K, typename V>
using pair_set = unordered_set<pair<K, V>>;

template <typename T, typename U>
struct QueryResult {
  unordered_set<T> firstArgVals;
  unordered_set<U> secondArgVals;
  pair_set<T, U> pairVals;
  bool isEmpty = true;

  void add(T first, U second) {
    firstArgVals.insert(first);
    secondArgVals.insert(second);
    pairVals.insert(std::make_pair(first, second));
    isEmpty = false;
  }
};