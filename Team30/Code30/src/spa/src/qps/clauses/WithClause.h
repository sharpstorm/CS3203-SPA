#pragma once

#include <memory>
#include <unordered_set>
#include <string>

#include "Clause.h"
#include "qps/clauses/arguments/WithArgument.h"

using std::unique_ptr, std::string;

using PKBAttributeQuerier = string(*)(PkbQueryHandler* queryHandler, const int &stmt);

typedef vector<int> StmtList;
typedef unordered_map<string, StmtList> SynToStmtMap;

template <class T, class U, class V>
using SynStmtMapExtractor = const T*(*)(const U* first, const V* second);

class WithClause: public Clause {
  WithArgumentPtr leftArg;
  WithArgumentPtr rightArg;
 public:
  WithClause(WithArgumentPtr left, WithArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override;
  bool validateArgTypes(VariableTable* variables) override;

  template <PKBAttributeQuerier querier, PQLSynonymType synType>
  void queryPkbForAttribute(PkbQueryHandler *pkbQueryHandler,
                            SynToStmtMap *map);
  bool populateMap(PQLSynonymType type, SynToStmtMap *map,
                   PkbQueryHandler *pkbQueryHandler);

 private:
  bool isEmptyResult();
  void evaluateOnIntAttributes(PQLQueryResult *result, PkbQueryHandler *pkbQueryHandler);
  void evaluateOnStringAttributes(PQLQueryResult *result, PkbQueryHandler *pkbQueryHandler);

  template <class R1, class R2, class U, class V, class T1, class T2>
  pair_set<R1, R2> crossMaps(SynToStmtMap* map1,
                                SynToStmtMap* map2,
                                SynStmtMapExtractor<T1, U, V> leftExtractor,
                                SynStmtMapExtractor<T2, U, V> rightExtractor) {
    pair_set<R1, R2> result;
    for (auto it1 = map1->begin(); it1 != map1->end(); it1++) {
      auto map2FindIterator = map2->find(it1->first);
      if (map2FindIterator == map2->end()) {
        continue;
      }

      crossProduct(leftExtractor(&(it1->first), &(it1->second)),
                   rightExtractor(&(map2FindIterator->first), &(map2FindIterator)->second),
                   &result);
    }

    return result;
  }

  void crossProduct(const StmtList* set1, const StmtList* set2, pair_set<int, int>* output) {
    for (auto it1 = set1->begin(); it1 != set1->end(); it1++) {
      for (auto it2 = set2->begin(); it2 != set2->end(); it2++) {
        output->insert({*it1, *it2});
      }
    }
  }

  void crossProduct(const string* set1, const StmtList* set2, pair_set<string, int>* output) {
    for (auto it = set2->begin(); it != set2->end(); it++) {
      output->insert({*set1, *it});
    }
  }

  void crossProduct(const StmtList* set1, const string* set2, pair_set<int, string>* output) {
    for (auto it = set1->begin(); it != set1->end(); it++) {
      output->insert({*it, *set2});
    }
  }

  void crossProduct(const string* set1, const string* set2, pair_set<string, string>* output) {
    output->insert({*set1, *set2});
  }
  SynonymList getUsedSynonyms() override;
};

typedef unique_ptr<WithClause> WithClausePtr;
