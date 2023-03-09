#pragma once

#include <memory>

#include "Clause.h"

using std::unique_ptr;

class WithClause: public Clause {
  ClauseArgumentPtr leftArg;
  ClauseArgumentPtr rightArg;
 public:
  WithClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;
  bool validateArgTypes(VariableTable* variables) override;
  SynonymList getUsedSynonyms() override;
};

typedef unique_ptr<WithClause> WithClausePtr;
