#pragma once

#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQLTypes.h"
#include "PQLQueryVariable.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::pair, std::string, std::unordered_map, std::unordered_set;

class StatementResult {
 public:
  unordered_set<int> lines;
  pair_set<int, int> linePairs;
  bool isLeftArg;
  bool isEmpty();
};

struct EntityResult {
 public:
  unordered_set<int> lines;
  unordered_set<string> entities;
  pair_set<int, string> enitityPairs;
  bool isLeftArg;
  bool isEmpty();
};

typedef unordered_map<PQL_VAR_NAME, StatementResult> STATEMENT_MAP;
typedef unordered_map<PQL_VAR_NAME, EntityResult> ENTITY_MAP;

class PQLQueryResult {
  STATEMENT_MAP statementMap;
  ENTITY_MAP entityMap;
  string error;
  bool isStaticFalse;

 public:
  PQLQueryResult();
  bool isStatementMapEmpty();
  bool isEntityMapEmpty();
  STATEMENT_MAP getStatementMap();
  ENTITY_MAP getEntityMap();
  void addToStatementMap(PQL_VAR_NAME name, StatementResult result);
  void addToEntityMap(PQL_VAR_NAME name, EntityResult result);
  StatementResult* getFromStatementMap(PQL_VAR_NAME var);
  EntityResult* getFromEntityMap(PQL_VAR_NAME var);
  string getError();
  void setError(string errorMessage);
  bool getIsStaticFalse();
  void setIsStaticFalse(bool staticRes);
  PQLQueryResult* resultFromVariable(PQLQueryVariable queryVar);
  virtual ~PQLQueryResult() = default;
};

