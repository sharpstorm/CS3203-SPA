#pragma once

#include <vector>
#include <queue>

#include "QueryGroupPlan.h"
#include "qps/common/PQLQuery.h"
#include "QueryGrouperIndex.h"
#include "QueryGroup.h"

using std::vector, std::queue;

class QueryGrouper {
 public:
  explicit QueryGrouper(const PQLQuery *query);
  vector<QueryGroupPtr> groupClauses();

 private:
  const PQLQuery *query;
  QueryGrouperIndex groupIndex;
  vector<IEvaluatable *> evaluatables;
  vector<int> groupClauseIdTable;
  PQLSynonymNameSet seenSynonyms;

  void initIndex();
  void findGroups(vector<QueryGroupPtr> *result);
  void findIndependentSelects(vector<QueryGroupPtr> *result);
  void selectAllDeclarations(vector<QueryGroupPtr> *result);
  QueryGroupPtr makeSelectClause(const PQLSynonymName &name);

  QueryGroup *BFSFindDependents(const int &start);
  void queueClauses(queue<PlanNode> *target, PlanNodes *values,
                    QueryGroup *result, const int &parentClauseId);
  void registerSeenSynonym(const PQLSynonymName &name, QueryGroup *result);
};
