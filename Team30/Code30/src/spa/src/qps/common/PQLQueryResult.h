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

typedef unordered_map<PQLSynonymName, StatementResult> STATEMENT_MAP;
typedef unordered_map<PQLSynonymName, EntityResult> ENTITY_MAP;

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
  void addToStatementMap(PQLSynonymName name, StatementResult result);
  void addToEntityMap(PQLSynonymName name, EntityResult result);
  StatementResult* getFromStatementMap(PQLSynonymName var);
  EntityResult* getFromEntityMap(PQLSynonymName var);
  string getError();
  void setError(string errorMessage);
  bool getIsStaticFalse();
  void setIsStaticFalse(bool staticRes);
  bool isStaticResult();
  PQLQueryResult* filterResultTo(PQLQuerySynonym queryVar);
  bool operator == (PQLQueryResult pqr) const;
  virtual ~PQLQueryResult() = default;
};

