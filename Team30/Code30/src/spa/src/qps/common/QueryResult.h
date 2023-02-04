#pragma once

#include <utility>
#include <string>
#include <unordered_map>
#include <vector>

#include "PQLTypes.h"
#include "PQLQueryVariable.h"

using std::pair, std::string, std::unordered_map, std::vector;

// Temporary Results
// TODO(KwanHW): Integrate when PKB comes in
struct StatementResult {
  vector<int> lines;
  vector<pair<int, int>> linePairs;
  bool isStaticTrue;
};

struct EntityResult {
  vector<int> lines;
  vector<string> entities;
  vector<pair<int, string>> enitityPairs;
  bool isStaticTrue;
};

typedef unordered_map<PQL_VAR_NAME, StatementResult> STATEMENT_MAP;
typedef unordered_map<PQL_VAR_NAME, EntityResult> ENTITY_MAP;

class QueryResult {
  STATEMENT_MAP statementMap;
  ENTITY_MAP entityMap;
  string error;
  bool isStaticTrue;

 public:
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
  bool getIsStaticTrue();
  void setIsStaticTrue(bool staticRes);
  QueryResult* resultFromVariable(PQLQueryVariable queryVar);
  virtual ~QueryResult() = default;
};

