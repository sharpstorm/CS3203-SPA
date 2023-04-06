#pragma once

#include <memory>

#include "qps/clauses/Clause.h"

using std::unique_ptr;

class PatternClause : public Clause {
 public:
  PatternClause(const PQLQuerySynonymProxy &synonym,
                ClauseArgumentPtr leftArg,
                PQLSynonymType synType);
  const PQLSynonymNameList getUsedSynonyms() const override;
  bool validateArgTypes(const VariableTable *variables) const override;

 protected:
  PQLQuerySynonymProxy synonym;
  ClauseArgumentPtr leftArg;

 private:
  PQLSynonymType synType;
};

typedef unique_ptr<PatternClause> PatternClausePtr;
