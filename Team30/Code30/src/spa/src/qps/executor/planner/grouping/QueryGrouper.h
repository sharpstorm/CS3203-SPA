#pragma once

#include <vector>
#include <queue>

#include "QueryGroupPlan.h"
#include "qps/common/PQLQuery.h"
#include "QueryGrouperIndex.h"
#include "QueryGroup.h"

using std::vector, std::queue;

class QueryGrouper {
  typedef int ClauseId;

 public:
  explicit QueryGrouper(const PQLQuery *query);
  QueryGroupPtrList groupClauses();

 private:
  const PQLQuery *query;
  QueryGrouperIndex groupIndex;
  IEvaluatableRefList evaluatables;
  vector<ClauseId> groupClauseIdTable;
  PQLSynonymNameSet seenSynonyms;

  void initIndex();
  void insertSelections(const AttributedSynonymList *selections);

  void findGroups(QueryGroupPtrList *result);
  void findIndependentSelects(QueryGroupPtrList *result);
  QueryGroupPtr makeSelectClause(const PQLSynonymName &name);

  QueryGroup *BFSFindDependents(const PlanNode start);
  void queueClauses(queue<PlanNode> *target, PlanNodes *values,
                    QueryGroup *result, const ClauseId parentClauseId);
  void registerSeenSynonym(const PQLSynonymName &name, QueryGroup *result);
};
