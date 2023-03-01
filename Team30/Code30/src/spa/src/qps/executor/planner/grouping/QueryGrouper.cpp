#include <memory>
#include <utility>

#include "QueryGrouper.h"
#include "qps/clauses/SelectClause.h"

const int UNPROCESSED = -2;
const int NO_GROUP = -1;

using std::make_shared, std::move;

QueryGrouper::QueryGrouper(PQLQuery *query) :
    query(query),
    evaluatables(query->getEvaluatables()),
    groupClauseIdTable(query->getClauseCount()) {}

vector<QueryGroupPtr> QueryGrouper::groupClauses() {
  vector<QueryGroupPtr> groups;
  initIndex();
  findGroups(&groups);
  findIndependentSelects(&groups);

  return groups;
}

void QueryGrouper::initIndex() {
  for (int i = 0; i < evaluatables.size(); i++) {
    groupClauseIdTable[i] = UNPROCESSED;
    for (PQLSynonymName name : evaluatables[i]->getUsedSynonyms()) {
      groupIndex.insertUsage(name, i);
    }
  }

  PQLQuerySynonymList* selections = query->getResultVariables();
  for (auto it = selections->begin(); it != selections->end(); it++) {
    groupIndex.insertSelection(it->getName());
  }
}

void QueryGrouper::findGroups(vector<QueryGroupPtr>* result) {
  for (int i = 0; i < groupClauseIdTable.size(); i++) {
    if (groupClauseIdTable[i] != UNPROCESSED) {
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
      registerSeenSynonym(name, result);
      queueClauses(&pendingNodes, groupIndex.getUsages(name),
                   result, clauseId);
    }
  }

  return result;
}

void QueryGrouper::registerSeenSynonym(PQLSynonymName name, QueryGroup *result) {
  if (seenSynonyms.find(name) == seenSynonyms.end()) {
    seenSynonyms.insert(name);
    if (groupIndex.selectSynonym(name)) {
      result->addSelectable(name);
    }
  }
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
    if (clauseId == UNPROCESSED) {
      groupClauseIdTable[nodeId] = NO_GROUP;
      target->push(nodeId);
    } else if (clauseId > NO_GROUP) {
      result->linkEvaluatables(parentClauseId, clauseId);
    }
  }
}

void QueryGrouper::findIndependentSelects(vector<QueryGroupPtr> *result) {
  unordered_set<PQLSynonymName>* unselected = groupIndex.getSelectSynonyms();
  for (auto it = unselected->begin(); it != unselected->end(); it++) {
    PQLQuerySynonym* synonym = query->getVariable(*it);
    IEvaluatableSPtr selectClause = make_shared<SelectClause>(*synonym);

    QueryGroupPtr selectGroup = make_unique<QueryGroup>();
    selectGroup->addEvaluatable(selectClause);
    selectGroup->addSelectable(synonym->getName());
    result->push_back(move(selectGroup));
  }
}
