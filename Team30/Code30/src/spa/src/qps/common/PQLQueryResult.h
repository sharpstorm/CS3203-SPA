#pragma once

#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "EntityResult.h"
#include "PQLTypes.h"
#include "PQLQuerySynonym.h"
#include "StatementResult.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"

using std::pair, std::string, std::unordered_map, std::unordered_set;

typedef unordered_map<PQL_SYN_NAME, StatementResult> STATEMENT_MAP;
typedef unordered_map<PQL_SYN_NAME, EntityResult> ENTITY_MAP;

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
  void addToStatementMap(PQL_SYN_NAME name, StatementResult result);
  void addToEntityMap(PQL_SYN_NAME name, EntityResult result);
  StatementResult* getFromStatementMap(PQL_SYN_NAME var);
  EntityResult* getFromEntityMap(PQL_SYN_NAME var);
  string getError();
  void setError(string errorMessage);
  bool getIsStaticFalse();
  void setIsStaticFalse(bool staticRes);
  bool isStaticResult();
  PQLQueryResult* filterResultTo(PQLQuerySynonym queryVar);
  bool operator == (PQLQueryResult pqr) const;
  virtual ~PQLQueryResult() = default;
};

