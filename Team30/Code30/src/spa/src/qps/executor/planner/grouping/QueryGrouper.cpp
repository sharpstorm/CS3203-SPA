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

QueryGroupPtrList QueryGrouper::groupClauses() {
  QueryGroupPtrList groups;
  initIndex();

  findGroups(&groups);
  findIndependentSelects(&groups);

  return groups;
}

void QueryGrouper::initIndex() {
  for (int i = 0; i < evaluatables.size(); i++) {
    groupClauseIdTable[i] = UNPROCESSED;
    for (const PQLSynonymName &name : evaluatables[i]->getUsedSynonyms()) {
      groupIndex.insertUsage(name, i);
    }
  }

  insertSelections(query->getResultVariables());
  PQLSynonymNameListPtr constrainedSyns = query->getConstrainedVariables();
  for (const PQLSynonymName &synName : *constrainedSyns) {
    groupIndex.insertConstraint(synName);
  }
}

void QueryGrouper::findGroups(QueryGroupPtrList *result) {
  for (int i = 0; i < groupClauseIdTable.size(); i++) {
    if (groupClauseIdTable[i] != UNPROCESSED) {
      continue;
    }

    QueryGroup *group = BFSFindDependents(i);
    result->push_back(QueryGroupPtr(group));
  }
}

QueryGroup *QueryGrouper::BFSFindDependents(const PlanNode start) {
  QueryGroup *result = new QueryGroup();
  queue<PlanNode> pendingNodes;
  pendingNodes.push(start);

  while (!pendingNodes.empty()) {
    int node = pendingNodes.front();
    pendingNodes.pop();

    ClauseId clauseId = result->addEvaluatable(evaluatables[node]);
    groupClauseIdTable[node] = clauseId;

    PQLSynonymNameList synonyms = evaluatables[node]->getUsedSynonyms();
    for (const PQLSynonymName &name : synonyms) {
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
                                const ClauseId parentClauseId) {
  if (values == nullptr) {
    return;
  }

  for (auto it = values->begin(); it != values->end(); it++) {
    PlanNode nodeId = *it;
    ClauseId clauseId = groupClauseIdTable[nodeId];
    if (clauseId == UNPROCESSED) {
      groupClauseIdTable[nodeId] = NO_GROUP;
      target->push(nodeId);
    } else if (clauseId > NO_GROUP) {
      result->linkEvaluatables(parentClauseId, clauseId);
    }
  }
}

void QueryGrouper::findIndependentSelects(QueryGroupPtrList *result) {
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

void QueryGrouper::insertSelections(const AttributedSynonymList *selections) {
  for (auto it = selections->begin(); it != selections->end(); it++) {
    groupIndex.insertSelection(it->getName());
  }
}
