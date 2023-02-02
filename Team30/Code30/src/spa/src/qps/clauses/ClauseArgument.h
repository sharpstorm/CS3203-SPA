#pragma once

#include "../common/PQLQueryVariable.h"

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
  ClauseArgument(ClauseArgumentType type);
  ClauseArgument(PQLQueryVariable variable);
  ClauseArgument(int stmt);
  ClauseArgument(string ident);

  bool isWildcard();
  bool isSynonym();
  bool isStmtRef();
  bool isEntRef();
};
