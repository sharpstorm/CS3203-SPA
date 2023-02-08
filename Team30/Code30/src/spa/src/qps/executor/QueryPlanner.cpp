#include "QueryPlanner.h"
#include "../clauses/SelectClause.h"

using std::make_shared;

shared_ptr<QueryPlan> QueryPlanner::getExecutionPlan(PQLQuery* query) {
  vector<shared_ptr<IEvaluatable>> queryClauses = query->getEvaluatables();

  bool foundUsingResult = false;
  for (int i = 0; i < query->getEvaluatables().size(); i++) {
    shared_ptr<IEvaluatable> clause = queryClauses.at(i);
    if (clause->usesSynonym(query->getResultName())) {
      foundUsingResult = true;
      break;
    }
  }

  if (!foundUsingResult) {
    shared_ptr<IEvaluatable> selectClause = make_shared<SelectClause>(
        query->getResultVariable());
    return make_shared<QueryPlan>(selectClause, query->getEvaluatables());
  }

  return make_shared<QueryPlan>(query->getEvaluatables());
}
