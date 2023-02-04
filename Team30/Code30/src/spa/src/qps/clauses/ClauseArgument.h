#pragma once

#include <string>
#include "../common/PQLQueryVariable.h"

using std::string;

enum ClauseArgumentType {
  CLAUSE_ARG_WILDCARD,
  CLAUSE_ARG_SYNONYM,
  CLAUSE_ARG_INTEGER,
  CLAUSE_ARG_STRING,
};

class ClauseArgument {
 private:
  ClauseArgumentType argType;
  PQLQueryVariable synonym;
  int statement;
  string ident;

 public:
  explicit ClauseArgument(ClauseArgumentType type);
  explicit ClauseArgument(PQLQueryVariable variable);
  explicit ClauseArgument(int stmt);
  explicit ClauseArgument(string ident);

  PQL_VAR_NAME getSynonymName();
  bool isWildcard();
  bool isSynonym();
  bool isStmtRef();
  bool isEntRef();
};