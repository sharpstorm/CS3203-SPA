#pragma once

#include <memory>

#include "Clause.h"
#include "qps/clauses/arguments/WithArgument.h"

using std::unique_ptr;

class WithClause : public Clause {
  AttributedSynonym leftArg;
  AttributedSynonym rightArg;

 public:
  WithClause(const AttributedSynonym &left, const AttributedSynonym &right);
  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override;
  bool validateArgTypes() const override;
  PQLSynonymNameList getUsedSynonyms() const override;
  ComplexityScore getComplexityScore(const OverrideTable *table) const override;
};

typedef unique_ptr<WithClause> WithClausePtr;
