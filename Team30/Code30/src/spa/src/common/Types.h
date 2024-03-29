#pragma once

#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using std::pair;
using std::set, std::unordered_set;
using std::string;
using std::unique_ptr;
using std::vector;

enum class EntityType { None, Procedure, Variable, Constant, Wildcard };
enum class StmtType { None, Read, Print, Assign, Call, While, If, Wildcard };

typedef string EntityValue;
typedef int StmtValue;
typedef int EntityIdx;
typedef int IntegerValue;
typedef unordered_set<StmtValue> StmtValueSet;
typedef unordered_set<EntityValue> EntityValueSet;
typedef unordered_set<EntityIdx> EntityIdxSet;
typedef set<StmtValue> StmtSet;
typedef set<EntityValue> EntitySet;
typedef vector<StmtValue> StmtValueList;

const StmtValue NO_STMT = 0;
const StmtValue INVALID_STMT = -1;
const IntegerValue NO_INT_VAL = -1;
const char NO_ENT[] = "";
const EntityIdx NO_ENT_INDEX = 0;

template<typename Value, typename Type>
class IRef {
 private:
  Type type;
  Value value;

 protected:
  IRef(const Type &type, const Value &value) : type(type), value(value) {}

 public:
  virtual ~IRef() = default;
  virtual bool isKnown() const = 0;
  bool isWildcard() const { return type == Type::Wildcard; }

  Value getValue() const { return value; }

  Type getType() const { return type; }

  bool isType(const Type &targetType) const { return type == targetType; }

  template <class ...T>
  bool isAnyType(const T&... types) const {
    return (isType(types) || ... || false);
  }

  void setType(Type newType) { type = newType; }
};

class StmtRef : public IRef<StmtValue, StmtType> {
 public:
  StmtRef(StmtType type, StmtValue lineNum);
  bool isKnown() const override;
  bool isValid() const;
};

class EntityRef : public IRef<EntityValue, EntityType> {
 public:
  explicit EntityRef(EntityType type);
  EntityRef(EntityType type, const EntityValue &name);

  bool isKnown() const override;
};

template<class T1, class T2>
struct std::hash<pair<T1, T2>> {
  std::size_t operator()(const pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};
template<typename K, typename V>
using pair_set = unordered_set<pair<K, V>>;

template<typename T>
using QueryResultSet = unordered_set<T>;

template<typename T, typename U>
using QueryResultPairSet = pair_set<T, U>;

template<typename T, typename U>
class QueryResult {
 private:
  QueryResultSet<T> firstArgVals;
  QueryResultSet<U> secondArgVals;
  QueryResultPairSet<T, U> pairVals;
  bool isEmpty = true;

 public:
  void addPair(const T &first, const U &second) {
    isEmpty = false;
    pairVals.insert(std::make_pair(first, second));
  }

  void addLeft(const T &first) {
    isEmpty = false;
    firstArgVals.insert(first);
  }

  void addRight(const U &second) {
    isEmpty = false;
    secondArgVals.insert(second);
  }

  void setNotEmpty() {
    isEmpty = false;
  }

  const QueryResultSet<T>& getLeftVals() const {
    return firstArgVals;
  }

  const QueryResultSet<U>& getRightVals() const {
    return secondArgVals;
  }

  const QueryResultPairSet<T, U>& getPairVals() const {
    return pairVals;
  }

  bool empty() const {
    return isEmpty;
  }
};

template<typename T, typename U>
using QueryResultPtr = unique_ptr<QueryResult<T, U>>;
