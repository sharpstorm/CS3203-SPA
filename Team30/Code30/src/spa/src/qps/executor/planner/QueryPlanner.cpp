#include "QueryPlanner.h"
#include "qps/common/IEvaluatable.h"
#include "qps/clauses/SelectClause.h"

using std::make_unique, std::make_shared;

QueryPlanner::QueryPlanner(PQLQuery *targetQuery):
    query(targetQuery),
    evaluatables(targetQuery->getEvaluatables()),
    finalPlan(vector<IEvaluatableSPtr>(
        targetQuery->getClauseCount())),
    mergeStrategy(vector<QueryPlan::MergeStrategy>(
        targetQuery->getClauseCount())),
    isEvaluatableLinked(vector<bool>(
        targetQuery->getClauseCount())),
    currentPosition(targetQuery->getClauseCount() - 1),
    isPlanBuilt(false) {}

unique_ptr<QueryPlan> QueryPlanner::getExecutionPlan() {
  if (!isPlanBuilt) {
    buildPlan();
    isPlanBuilt = true;
  }

  return make_unique<QueryPlan>(finalPlan, mergeStrategy);
}

void QueryPlanner::buildPlan() {
  initState();
  PQLSynonymName selectingName = query->getResultName();
  PlanNodes* nodes = planIndex.getUsages(selectingName);

  buildDependencyTree(nodes);
  flushIndependentClauses();

  bool isSelectIndependent = nodes == nullptr || nodes->empty();
  if (isSelectIndependent) {
    IEvaluatableSPtr selectClause = make_shared<SelectClause>(
        query->getResultVariable());
    finalPlan.push_back(selectClause);
    if (!evaluatables.empty()) {
      mergeStrategy[evaluatables.size() - 1] = QueryPlan::DISCARDING_MERGE;
    }
  } else {
    mergeStrategy.pop_back();
  }
}

void QueryPlanner::initState() {
  for (int i = 0; i < evaluatables.size(); i++) {
    isEvaluatableLinked[i] = false;
    for (PQLSynonymName name : evaluatables[i]->getUsedSynonyms()) {
      planIndex.insertUsage(name, i);
    }
  }
}

void QueryPlanner::buildDependencyTree(PlanNodes* startingNodes) {
  queue<PlanNode> pendingNodes;
  insertIntoQueue(&pendingNodes, startingNodes);

  while (!pendingNodes.empty()) {
    int node = pendingNodes.front();
    pendingNodes.pop();

    if (isEvaluatableLinked[node]) {
      continue;
    }

    isEvaluatableLinked[node] = true;
    finalPlan[currentPosition] = evaluatables[node];
    mergeStrategy[currentPosition] = QueryPlan::INNER_JOIN;
    currentPosition--;

    SynonymList synonyms = evaluatables[node]->getUsedSynonyms();
    for (PQLSynonymName name : synonyms) {
      insertIntoQueue(&pendingNodes, planIndex.getUsages(name));
    }
  }
}

void QueryPlanner::flushIndependentClauses() {
  for (int i = 0; i < isEvaluatableLinked.size(); i++) {
    if (isEvaluatableLinked[i]) {
      continue;
    }

    finalPlan[currentPosition] = evaluatables.at(i);
    mergeStrategy[currentPosition] = QueryPlan::DISCARDING_MERGE;
    currentPosition--;
  }
}

void QueryPlanner::insertIntoQueue(queue<PlanNode> *target,
                                   PlanNodes* values) {
  if (values == nullptr) {
    return;
  }

  for (auto it = values->begin(); it != values->end(); it++) {
    target->push(*it);
  }
}
