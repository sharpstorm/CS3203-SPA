#pragma once

#include <memory>

#include "Clause.h"
#include "qps/clauses/arguments/WithArgument.h"

using std::unique_ptr;

class WithClause: public Clause {
  AttributedSynonym leftArg;
  AttributedSynonym rightArg;

 public:
  WithClause(const AttributedSynonym &left, const AttributedSynonym &right);
  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override;
  bool validateArgTypes(VariableTable* variables) override;
  SynonymList getUsedSynonyms() override;
  ComplexityScore getComplexityScore(const OverrideTable *table) override;
};

typedef unique_ptr<WithClause> WithClausePtr;
