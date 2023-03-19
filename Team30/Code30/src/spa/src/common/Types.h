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

template<typename Value, typename Type>
struct IRef {
 public:
  virtual ~IRef() {}
  virtual bool isKnown() const = 0;
  virtual Value getValue() const = 0;
  virtual Type getType() const = 0;
  virtual void setType(Type) = 0;
};

struct StmtRef : public IRef<StmtValue, StmtType> {
  StmtType type;
  StmtValue lineNum;
  StmtRef(StmtType type, StmtValue lineNum) : type(type), lineNum(lineNum) {}
  bool isKnown() const override { return lineNum != 0; }
  StmtValue getValue() const override { return lineNum; }
  StmtType getType() const override { return type; }
  void setType(StmtType newType) override {
    type = newType;
  }
};

struct EntityRef : public IRef<EntityValue, EntityType> {
  EntityType type;
  EntityValue name;
  explicit EntityRef(EntityType type) : type(type) {}
  EntityRef(EntityType type, EntityValue name) : type(type), name(name) {}
  bool isKnown() const override { return !name.empty(); }
  EntityValue getValue() const override { return name; }
  EntityType getType() const override { return type; }
  void setType(EntityType newType) override {
    type = newType;
  }
};

template<class T1, class T2>
struct std::hash<pair<T1, T2>> {
  std::size_t operator()(const pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};
template<typename K, typename V>
using pair_set = unordered_set<pair<K, V>>;

template<typename T, typename U>
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
