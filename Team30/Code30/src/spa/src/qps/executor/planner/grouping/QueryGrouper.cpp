#include <memory>
#include <utility>

#include "QueryGrouper.h"
#include "qps/clauses/SelectClause.h"

const int UNPROCESSED = -2;
const int NO_GROUP = -1;

using std::make_unique;

QueryGrouper::QueryGrouper(const PQLQuery *query) :
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

  const AttributedSynonymList *selections = query->getResultVariables();
  for (auto it = selections->begin(); it != selections->end(); it++) {
    groupIndex.insertSelection(it->getName());
  }

  auto constraints = query->getConstraints();
  for (IConstraint *constraint : constraints) {
    for (PQLSynonymName syn : constraint->getAffectedSyns()) {
      groupIndex.insertConstraint(syn);
    }
  }
}

void QueryGrouper::findGroups(vector<QueryGroupPtr> *result) {
  for (int i = 0; i < groupClauseIdTable.size(); i++) {
    if (groupClauseIdTable[i] != UNPROCESSED) {
      continue;
    }

    QueryGroup *group = BFSFindDependents(i);
    result->push_back(QueryGroupPtr(group));
  }
}

QueryGroup *QueryGrouper::BFSFindDependents(const int start) {
  QueryGroup *result = new QueryGroup();
  queue<int> pendingNodes;
  pendingNodes.push(start);

  while (!pendingNodes.empty()) {
    int node = pendingNodes.front();
    pendingNodes.pop();

    int clauseId = result->addEvaluatable(evaluatables[node]);
    groupClauseIdTable[node] = clauseId;

    PQLSynonymNameList synonyms = evaluatables[node]->getUsedSynonyms();
    for (PQLSynonymName name : synonyms) {
      registerSeenSynonym(name, result);
      queueClauses(&pendingNodes, groupIndex.getUsages(name),
                   result, clauseId);
    }
  }

  return result;
}

void QueryGrouper::registerSeenSynonym(const PQLSynonymName &name,
                                       QueryGroup *result) {
  if (seenSynonyms.find(name) == seenSynonyms.end()) {
    seenSynonyms.insert(name);
    groupIndex.linkConstraint(name);
    if (groupIndex.selectSynonym(name)) {
      result->addSelectable(name);
    }
  }
}

void QueryGrouper::queueClauses(queue<PlanNode> *target,
                                PlanNodes *values,
                                QueryGroup *result,
                                const int parentClauseId) {
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
  const PQLSynonymNameSet *unselected = groupIndex.getHangingSelects();
  for (const auto &name : *unselected) {
    QueryGroupPtr selectGroup = makeSelectClause(name);
    selectGroup->addSelectable(name);
    result->push_back(std::move(selectGroup));
  }

  const PQLSynonymNameSet *hangingConstraints =
      groupIndex.getHangingConstraints();
  for (const auto &name : *hangingConstraints) {
    QueryGroupPtr selectGroup = makeSelectClause(name);
    result->push_back(std::move(selectGroup));
  }
}

QueryGroupPtr QueryGrouper::makeSelectClause(const PQLSynonymName &name) {
  PQLQuerySynonymProxy *synProxy = query->getVariable(name);
  IEvaluatablePtr selectClause = make_unique<SelectClause>(*synProxy);

  QueryGroupPtr selectGroup = make_unique<QueryGroup>();
  selectGroup->addEvaluatable(std::move(selectClause));
  return selectGroup;
}
