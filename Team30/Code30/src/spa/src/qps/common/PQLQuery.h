#pragma once

#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "qps/clauses/Clause.h"
#include "AttributedSynonym.h"
#include "IConstraint.h"
#include "qps/common/VariableTable.h"

using std::vector, std::unique_ptr;

class PQLQuery {
 private:
  VariableTablePtr variables;
  AttributedSynonymList resultVariables;
  vector<ClausePtr> clauses;
  vector<ConstraintPtr> constraints;

  bool hasAppliedConstraints;

 public:
  PQLQuery(VariableTablePtr vars,
           const AttributedSynonymList &resVars,
           vector<ClausePtr> c,
           vector<ConstraintPtr> con);

  int getDeclaredVariableCount() const;
  const AttributedSynonymList *getResultVariables() const;
  const PQLSynonymNameListPtr getConstrainedVariables() const;

  PQLQuerySynonymProxy *getVariable(const PQLSynonymName &name) const;
  const vector<IEvaluatable *> getEvaluatables() const;
  bool resolveConstraints(OverrideTable *tableOut);

  int getClauseCount() const;
  bool isBooleanResult() const;
};

typedef unique_ptr<PQLQuery> PQLQueryPtr;
