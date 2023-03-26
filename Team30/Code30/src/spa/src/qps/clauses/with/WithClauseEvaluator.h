#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/constraint/OverrideTable.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/arguments/WithArgument.h"
#include "qps/executor/QueryExecutorAgent.h"

using PKBAttributeQuerier =
    EntityValue(*)(const QueryExecutorAgent &agent, const int &stmt);

typedef vector<int> StmtList;
typedef unordered_map<EntityValue, StmtList> SynToStmtMap;

template <class T, class U, class V>
using SynStmtMapExtractor = const T*(*)(const U* first, const V* second);

class WithClauseEvaluator {
 public:
  WithClauseEvaluator(const QueryExecutorAgent &agent,
                      AttributedSynonym* leftArg,
                      AttributedSynonym* rightArg);
  PQLQueryResult* evaluate();

 private:
  const QueryExecutorAgent &agent;
  AttributedSynonym* leftArg;
  AttributedSynonym* rightArg;

  PQLQueryResult* result;

  bool isEmptyResult();
  bool isIntegerIndependent(const PQLSynonymType &type);

  void evaluateOnIntAttributes();
  void evaluateOnStringAttributes();

  void evaluateOnStmtConst(AttributedSynonym* constant,
                           AttributedSynonym* stmt);
  void evaluateOnStmtStmt();

  bool populateMap(const PQLQuerySynonymProxy &arg, SynToStmtMap *map);
  template <PKBAttributeQuerier querier>
  void queryPkbForAttribute(SynToStmtMap *map, const PQLQuerySynonymProxy &arg);

  StmtValueSet queryForStatement(const PQLQuerySynonymProxy &syn);
  EntityValueSet queryForEntity(const PQLQuerySynonymProxy &syn);

  template <class T>
  void addToResult(const T &pkbResult) {
    result->add(leftArg->getName(), rightArg->getName(), pkbResult);
  }

  template <class R1, class R2, class U, class V, class T1, class T2>
  pair_set<R1, R2> crossMaps(SynToStmtMap* map1,
                             SynToStmtMap* map2,
                             SynStmtMapExtractor<T1, U, V> leftExtractor,
                             SynStmtMapExtractor<T2, U, V> rightExtractor) {
    pair_set<R1, R2> pairResult;
    for (auto it1 = map1->begin(); it1 != map1->end(); it1++) {
      auto map2FindIterator = map2->find(it1->first);
      if (map2FindIterator == map2->end()) {
        continue;
      }

      crossProduct(leftExtractor(&(it1->first), &(it1->second)),
                   rightExtractor(&(map2FindIterator->first),
                                  &(map2FindIterator)->second),
                   &pairResult);
    }

    return pairResult;
  }

  void crossProduct(const StmtList* set1, const StmtList* set2,
                    pair_set<StmtValue, StmtValue>* output) {
    for (auto it1 = set1->begin(); it1 != set1->end(); it1++) {
      for (auto it2 = set2->begin(); it2 != set2->end(); it2++) {
        output->insert({*it1, *it2});
      }
    }
  }

  void crossProduct(const EntityValue* set1, const StmtList* set2,
                    pair_set<EntityValue, StmtValue>* output) {
    for (auto it = set2->begin(); it != set2->end(); it++) {
      output->insert({*set1, *it});
    }
  }

  void crossProduct(const StmtList* set1, const EntityValue* set2,
                    pair_set<StmtValue, EntityValue>* output) {
    for (auto it = set1->begin(); it != set1->end(); it++) {
      output->insert({*it, *set2});
    }
  }

  void crossProduct(const EntityValue* set1,
                    const EntityValue* set2,
                    pair_set<EntityValue, EntityValue>* output) {
    output->insert({*set1, *set2});
  }
};
