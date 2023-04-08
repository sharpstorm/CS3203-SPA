#pragma once

#include <vector>
#include <memory>

#include "PQLTypes.h"
#include "qps/clauses/Clause.h"
#include "AttributedSynonym.h"
#include "qps/constraints/Constraint.h"
#include "qps/common/VariableTable.h"

using std::vector, std::unique_ptr;

class PQLQuery {
 private:
  VariableTablePtr variables;
  AttributedSynonymList resultVariables;
  vector<ClausePtr> clauses;
  vector<ConstraintPtr> constraints;

 public:
  PQLQuery(VariableTablePtr vars,
           const AttributedSynonymList &resVars,
           vector<ClausePtr> c,
           vector<ConstraintPtr> con);

  VariableTable *getVarTable() const;
  const AttributedSynonymList *getResultVariables() const;

  PQLQuerySynonymProxy *getVariable(const PQLSynonymName &name) const;
  const vector<IEvaluatable *> getEvaluatables() const;
  const vector<Constraint *> getConstraints() const;

  int getClauseCount() const;
  bool isBooleanResult() const;
};

typedef unique_ptr<PQLQuery> PQLQueryPtr;
