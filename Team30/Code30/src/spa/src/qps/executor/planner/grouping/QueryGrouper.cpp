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

  if (!groupIndex.hasSelectables() && query->getClauseCount() == 0) {
    selectAllDeclarations(&groups);
  } else {
    findGroups(&groups);
    findIndependentSelects(&groups);
  }

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
  for (Constraint *constraint : constraints) {
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

QueryGroup *QueryGrouper::BFSFindDependents(const int &start) {
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
    if (groupIndex.selectSynonym(name)) {
      result->addSelectable(name);
    }
  }
}

void QueryGrouper::queueClauses(queue<PlanNode> *target,
                                PlanNodes *values,
                                QueryGroup *result,
                                const int &parentClauseId) {
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
  PQLSynonymNameSet *unselected = groupIndex.getSelectSynonyms();
  for (auto it = unselected->begin(); it != unselected->end(); it++) {
    PQLSynonymName name = *it;
    QueryGroupPtr selectGroup = makeSelectClause(name);
    selectGroup->addSelectable(name);
    result->push_back(std::move(selectGroup));
  }
}

void QueryGrouper::selectAllDeclarations(vector<QueryGroupPtr> *result) {
  for (PQLSynonymName name : query->getDeclaredSynonyms()) {
    result->push_back(makeSelectClause(name));
  }
}

QueryGroupPtr QueryGrouper::makeSelectClause(const PQLSynonymName &name) {
  PQLQuerySynonymProxy *synProxy = query->getVariable(name);
  IEvaluatablePtr selectClause = make_unique<SelectClause>(*synProxy);

  QueryGroupPtr selectGroup = make_unique<QueryGroup>(
      !groupIndex.isConstrained(name));
  selectGroup->addEvaluatable(std::move(selectClause));
  return selectGroup;
}
