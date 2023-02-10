#include <iostream>
#include "QueryPlanner.h"

#include <vector>
#include "../common/IEvaluatable.h"
#include "../clauses/SelectClause.h"

using std::make_unique, std::make_shared;

unique_ptr<QueryPlan> QueryPlanner::getExecutionPlan(PQLQuery* query) {
  bool isUsingResult = isResultConditioned(query);

  if (!isUsingResult) {
    shared_ptr<IEvaluatable> selectClause = make_shared<SelectClause>(
        query->getResultVariable());
    return make_unique<QueryPlan>(selectClause, query->getEvaluatables());
  }

  return make_unique<QueryPlan>(query->getEvaluatables());
}

bool QueryPlanner::isResultConditioned(PQLQuery *query) {
  vector<shared_ptr<IEvaluatable>> queryClauses = query->getEvaluatables();
  for (int i = 0; i < queryClauses.size(); i++) {
    shared_ptr<IEvaluatable> clause = queryClauses.at(i);
    if (clause->usesSynonym(query->getResultName())) {
      return true;
    }
  }
  return false;
}
