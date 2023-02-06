#include "QueryPlan.h"

QueryPlan::QueryPlan(
    shared_ptr<IEvaluatable> selectClause,
    vector<shared_ptr<IEvaluatable>> conditionalClauses):
    selectClause(selectClause), conditionalClauses(conditionalClauses) {}

QueryPlan::QueryPlan(vector<shared_ptr<IEvaluatable>> conditionalClauses):
    selectClause(nullptr), conditionalClauses(conditionalClauses) {}

bool QueryPlan::hasSelectClause() {
  return selectClause != nullptr;
}

shared_ptr<IEvaluatable> QueryPlan::getSelectClause() {
  return selectClause;
}

vector<shared_ptr<IEvaluatable>> QueryPlan::getConditionalClauses() {
  return conditionalClauses;
}
