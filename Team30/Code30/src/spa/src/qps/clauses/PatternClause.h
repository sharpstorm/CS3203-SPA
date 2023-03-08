#pragma once

#include <memory>

#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuery.h"
#include "qps/clauses/Clause.h"

using std::unique_ptr;

class PatternClause: virtual public Clause {
 public:
  PatternClause(const PQLQuerySynonym &synonym,
                ClauseArgumentPtr leftArg,
                PQLSynonymType synType);
  SynonymList getUsedSynonyms() override;
  bool validateArgTypes(VariableTable *variables) override;

 protected:
  PQLQuerySynonym synonym;
  ClauseArgumentPtr leftArg;

 private:
  PQLSynonymType synType;
};

typedef unique_ptr<PatternClause> PatternClausePtr;
