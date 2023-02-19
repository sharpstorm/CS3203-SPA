#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "QueryPlanner.h"
#include "../common/IEvaluatable.h"
#include "../clauses/SelectClause.h"

using std::make_unique, std::make_shared, std::queue;

unique_ptr<QueryPlan> QueryPlanner::getExecutionPlan(PQLQuery* query) {
  PlanNodeMap planIndex;
  vector<PlanNode> nodes;

  vector<IEvaluatableSPtr> evals = query->getEvaluatables();
  vector<IEvaluatableSPtr> finalPlan(evals.size());
  vector<QueryPlan::MergeStrategy> mergeStrategy(evals.size());
  vector<bool> isLinked(evals.size());

  int currentPosition = evals.size() - 1;

  for (int i = 0; i < evals.size(); i++) {
    isLinked[i] = false;
    SynonymList synonyms = evals[i]->getUsedSynonyms();
    for (PQLSynonymName name : synonyms) {
      if (planIndex.find(name) != planIndex.end()) {
        planIndex[name].insert(i);
      } else {
        planIndex[name] = unordered_set<int>{i};
      }
    }
  }

  PQLSynonymName selectingName = query->getResultName();
  queue<int> pendingNodes;
  insertIntoQueue(&pendingNodes, getOrNull(&planIndex, selectingName));
  bool isSelectIndependent = pendingNodes.empty();

  while (!pendingNodes.empty()) {
    int node = pendingNodes.front();
    pendingNodes.pop();

    if (isLinked[node]) {
      continue;
    }
    isLinked[node] = true;
    finalPlan[currentPosition] = evals.at(node);
    mergeStrategy[currentPosition] = QueryPlan::INNER_JOIN;
    currentPosition--;

    SynonymList synonyms = evals[node]->getUsedSynonyms();
    for (PQLSynonymName name : synonyms) {
      insertIntoQueue(&pendingNodes, getOrNull(&planIndex, name));
    }
  }

  for (int i = 0; i < isLinked.size(); i++) {
    if (!isLinked[i]) {
      finalPlan[currentPosition] = evals.at(i);
      mergeStrategy[currentPosition] = QueryPlan::DISCARDING_MERGE;
      currentPosition--;
    }
  }

  if (isSelectIndependent) {
    IEvaluatableSPtr selectClause = make_shared<SelectClause>(
        query->getResultVariable());
    finalPlan.push_back(selectClause);
    if (evals.size() > 0) {
      mergeStrategy[evals.size() - 1] = QueryPlan::DISCARDING_MERGE;
    }
  } else {
    mergeStrategy.pop_back();
  }

  return make_unique<QueryPlan>(finalPlan, mergeStrategy);
}

void QueryPlanner::insertIntoQueue(queue<int> *target,
                                   unordered_set<int>* values) {
  if (values == nullptr) {
    return;
  }

  for (auto it = values->begin(); it != values->end(); it++) {
    target->push(*it);
  }
}

PlanNode* QueryPlanner::getOrNull(PlanNodeMap* map,
                                  const string &key) {
  if (map->find(key) == map->end()) {
    return nullptr;
  }
  return &map->at(key);
}
