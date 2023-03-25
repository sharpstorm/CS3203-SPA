#pragma once

#include <memory>

#include "common/Types.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/interfaces/IProvider.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

using std::unique_ptr;

template <typename LeftValue, typename LeftType, typename RightValue,
          typename RightType>
class BaseQueryInvoker {
 private:
  IProvider<LeftValue, LeftType> *leftProvider;
  AbstractPredicateFactory<LeftValue, LeftType> *leftPredicateFactory;
  AbstractPredicateFactory<RightValue, RightType> *rightPredicateFactory;

 public:
  BaseQueryInvoker(
      IProvider<LeftValue, LeftType> *leftProvider,
      AbstractPredicateFactory<LeftValue, LeftType> *leftPredicateFactory,
      AbstractPredicateFactory<RightValue, RightType> *rightPredicateFactory)
      : leftProvider(leftProvider),
        leftPredicateFactory(leftPredicateFactory),
        rightPredicateFactory(rightPredicateFactory) {}

  unique_ptr<QueryResult<LeftValue, RightValue>> query(
      RelationTableManager<LeftValue, RightValue> *store,
      IRef<LeftValue, LeftType> *arg1,
      IRef<RightValue, RightType> *arg2) const {
    if (arg1->isKnown()) {
      return store->query(arg1->getValue(),
                          rightPredicateFactory->getPredicate(arg2));
    } else if (arg2->isKnown()) {
      return store->query(leftPredicateFactory->getPredicate(arg1),
                          arg2->getValue());
    } else {
      return store->query(leftProvider->getValuesOfType(arg1->getType()),
                          rightPredicateFactory->getPredicate(arg2));
    }
  }
};
