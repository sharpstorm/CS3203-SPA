#include "QueryGrouper.h"

const int NO_GROUP = -1;

QueryGrouper::QueryGrouper(PQLQuery *query) :
    query(query),
    evaluatables(query->getEvaluatables()),
    groupClauseIdTable(query->getClauseCount()) {}

vector<QueryGroupPtr> QueryGrouper::groupClauses() {
  vector<QueryGroupPtr> groups;
  initIndex();
  findGroups(&groups);

  return groups;
}

void QueryGrouper::initIndex() {
  for (int i = 0; i < evaluatables.size(); i++) {
    groupClauseIdTable[i] = NO_GROUP;
    for (PQLSynonymName name : evaluatables[i]->getUsedSynonyms()) {
      groupIndex.insertUsage(name, i);
    }
  }
}

void QueryGrouper::findGroups(vector<QueryGroupPtr>* result) {
  for (int i = 0; i < groupClauseIdTable.size(); i++) {
    if (groupClauseIdTable[i] != NO_GROUP) {
      continue;
    }

    QueryGroup* group = BFSFindDependents(i);
    result->push_back(QueryGroupPtr(group));
  }
}

QueryGroup *QueryGrouper::BFSFindDependents(int start) {
  QueryGroup* result = new QueryGroup();
  queue<int> pendingNodes;
  pendingNodes.push(start);

  while (!pendingNodes.empty()) {
    int node = pendingNodes.front();
    pendingNodes.pop();

    int clauseId = result->addEvaluatable(evaluatables[node]);
    groupClauseIdTable[node] = clauseId;

    SynonymList synonyms = evaluatables[node]->getUsedSynonyms();
    for (PQLSynonymName name : synonyms) {
      queueClauses(&pendingNodes, groupIndex.getUsages(name));
    }
  }

  return result;
}

void QueryGrouper::queueClauses(queue<PlanNode> *target,
                                       PlanNodes *values,
                                       QueryGroup *result,
                                       int parentClauseId) {
  if (values == nullptr) {
    return;
  }

  for (auto it = values->begin(); it != values->end(); it++) {
    int nodeId = *it;
    int clauseId = groupClauseIdTable[nodeId];
    if (clauseId == NO_GROUP) {
      target->push(nodeId);
    } else {
      result->linkEvaluatables(parentClauseId, clauseId);
    }
  }
}
