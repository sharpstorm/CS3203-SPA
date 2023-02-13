#pragma once

#include <string>
#include "../common/PQLQuerySynonym.h"

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
  PQLQuerySynonym synonym;
  int statement;
  string ident;

 public:
  explicit ClauseArgument(ClauseArgumentType type);
  explicit ClauseArgument(PQLQuerySynonym variable);
  explicit ClauseArgument(int stmt);
  explicit ClauseArgument(string ident);

  PQLSynonymType getSynonymType();
  PQLSynonymName getSynonymName();
  bool isWildcard();
  bool isSynonym();
  bool isStmtRef();
  bool isEntRef();
  int getStatement();
  string getIdent();
};
