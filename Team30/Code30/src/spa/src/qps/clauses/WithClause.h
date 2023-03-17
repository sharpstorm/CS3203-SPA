#pragma once

#include <memory>

#include "Clause.h"
#include "qps/clauses/arguments/WithArgument.h"

using std::unique_ptr;

class WithClause: public Clause {
  WithArgumentPtr leftArg;
  WithArgumentPtr rightArg;
 public:
  WithClause(WithArgumentPtr left, WithArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                             OverrideTable* table) override;
  bool validateArgTypes(VariableTable* variables) override;
  SynonymList getUsedSynonyms() override;
};

typedef unique_ptr<WithClause> WithClausePtr;
