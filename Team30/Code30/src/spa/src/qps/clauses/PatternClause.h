#pragma once

#include <memory>

#include "qps/common/PQLQueryResult.h"
#include "qps/common/PQLQuery.h"
#include "qps/clauses/Clause.h"

using std::unique_ptr;

class PatternClause: public Clause {
 public:
  PatternClause(PQLQuerySynonym synonym,
                ClauseArgumentPtr leftArg,
                PQLSynonymType synType);
  virtual ~PatternClause() = default;
  SynonymList getUsedSynonyms();
  bool validateArgTypes(VariableTable *variables);

 protected:
  PQLQuerySynonym synonym;
  ClauseArgumentPtr leftArg;

 private:
  PQLSynonymType synType;
};

typedef unique_ptr<PatternClause> PatternClausePtr;
