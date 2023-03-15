#pragma once

#include <vector>
#include <queue>
#include <unordered_set>

#include "qps/executor/planner/QueryGroupPlan.h"
#include "qps/common/PQLQuery.h"
#include "QueryGrouperIndex.h"
#include "QueryGroup.h"

using std::vector, std::queue, std::unordered_set;

class QueryGrouper {
 public:
  explicit QueryGrouper(PQLQuery* query);
  vector<QueryGroupPtr> groupClauses();

 private:
  PQLQuery* query;
  QueryGrouperIndex groupIndex;
  vector<IEvaluatableSPtr> evaluatables;
  vector<int> groupClauseIdTable;
  unordered_set<PQLSynonymName> seenSynonyms;

  void initIndex();
  void findGroups(vector<QueryGroupPtr>* result);
  void findIndependentSelects(vector<QueryGroupPtr>* result);
  void selectAllDeclarations(vector<QueryGroupPtr>* result);
  QueryGroupPtr makeSelectClause(const PQLSynonymName &name);

  QueryGroup* BFSFindDependents(int start);
  void queueClauses(queue<PlanNode> *target, PlanNodes *values,
                    QueryGroup *result, int parentClauseId);
  void registerSeenSynonym(PQLSynonymName name, QueryGroup *result);
};
