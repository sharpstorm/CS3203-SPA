#pragma once

#include <string>
#include <memory>
#include "common/Types.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/interfaces/IProvider.h"

using std::string;

// helper class to do query
class QueryHandlerHelper {
 public:
  QueryResult<int, int> query(RelationTableManager<int, int> *store,
                              StmtRef arg1, StmtRef arg2) {
    return query(store, &arg1, &arg2, structureMappingProvider,
                 structureMappingProvider);
  }

  QueryResult<int, string> query(RelationTableManager<int, string> *store,
                                 StmtRef arg1, EntityRef arg2) {
    return query(store, &arg1, &arg2, structureMappingProvider,
                 entityMappingProvider);
  }

  QueryResult<string, string> query(RelationTableManager<string, string> *store,
                                    EntityRef arg1, EntityRef arg2) {
    return query(store, &arg1, &arg2, entityMappingProvider,
                 entityMappingProvider);
  }

 private:
  IProvider<int, StmtType> *structureMappingProvider;
  IProvider<string, EntityType> *entityMappingProvider;
  PredicateFactory *predicateFactory;

  template <typename K, typename V, typename T, typename U>
  QueryResult<K, V> query(RelationTableManager<K, V> *store, IRef<T, K> *arg1,
                          IRef<U, V> *arg2, IProvider<K, T> *provider1,
                          IProvider<V, U> *provider2) {
    if (arg1->isKnown()) {
      return store->query(arg1->getValue(),
                          predicateFactory->getPredicate(arg2, provider2));
    } else if (arg2->isKnown()) {
      return store->query(predicateFactory->getPredicate(arg1, provider1),
                          arg2->getValue());
    } else {
      return store->query(provider1->getValuesOfType(arg1->getType()),
                          predicateFactory->getPredicate(arg2, provider2));
    }
  }
};
