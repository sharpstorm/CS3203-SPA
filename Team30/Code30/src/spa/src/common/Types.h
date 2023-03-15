#pragma once

#include <string>
#include <unordered_set>
#include <utility>

using std::pair;
using std::string;
using std::unordered_set;

enum class EntityType { None, Procedure, Variable, Constant };

enum class StmtType { None, Read, Print, Assign, Call, While, If };

typedef string EntityValue;
typedef int StmtValue;

template <typename T, typename V>
struct IRef {
 public:
  virtual ~IRef() {}
  virtual bool isKnown() const = 0;
  virtual V getValue() const = 0;
  virtual T getType() const = 0;
};

struct StmtRef : IRef<StmtType, int> {
  StmtType type;
  int lineNum;
  StmtRef(StmtType type, int lineNum) : type(type), lineNum(lineNum) {}
  bool isKnown() const override { return lineNum != 0; }
  int getValue() const override { return lineNum; }
  StmtType getType() const override { return type; }
};

struct EntityRef : IRef<EntityType, string> {
  EntityType type;
  string name;
  explicit EntityRef(EntityType type) : type(type) {}
  EntityRef(EntityType type, string name) : type(type), name(name) {}
  bool isKnown() const override { return !name.empty(); }
  string getValue() const override { return name; }
  EntityType getType() const override { return type; }
};

template <class T1, class T2>
struct std::hash<pair<T1, T2>> {
  std::size_t operator()(const pair<T1, T2> &pair) const {
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
