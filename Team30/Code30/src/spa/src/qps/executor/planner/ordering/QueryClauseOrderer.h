#pragma once

#include <queue>
#include <vector>
#include <unordered_set>

#include "qps/executor/planner/grouping/QueryGroup.h"
#include "qps/executor/planner/grouping/QueryGroupPlan.h"
#include "common/data_structs/BitField.h"

using std::priority_queue, std::vector, std::unordered_set;

typedef int ClauseId;

class QueryClauseOrderer {
 private:
  class ComparableClause {
   private:
    ComplexityScore complexity;
    ClauseId clauseId;

   public:
    ComparableClause(const ComplexityScore &complexity,
                     const ClauseId &clauseId):
        complexity(complexity), clauseId(clauseId) {
    }

    ClauseId getId() {
      return clauseId;
    }

    ComplexityScore getComplexity() {
      return complexity;
    }

    bool operator<(const ComparableClause& other) const {
      return complexity > other.complexity;
    }

    bool operator>(const ComparableClause& other) const {
      return complexity < other.complexity;
    }
  };

 public:
  QueryGroupPlanPtr orderClauses(QueryGroup* group, OverrideTable* overrides);
};
