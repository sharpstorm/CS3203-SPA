#pragma once

#include "common/Types.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/interfaces/IProvider.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

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

  QueryResultPtr<LeftValue, RightValue> query(
      RelationTableManager<LeftValue, RightValue> *store,
      IRef<LeftValue, LeftType> *arg1, IRef<RightValue, RightType> *arg2,
      bool returnAllResult) const {
    auto resultBuilder = QueryResultBuilder<LeftValue, RightValue>();
    if (returnAllResult) {
      resultBuilder.setAllVals();
    } else {
      if (!arg1->isKnown() && !arg2->isKnown()) {
        resultBuilder.setPairVals();
      } else if (arg1->isKnown() && !arg2->isKnown()) {
        resultBuilder.setRightVals();
      } else if (!arg1->isKnown() && arg2->isKnown()) {
        resultBuilder.setLeftVals();
      }
    }

    if (arg1->isKnown()) {
      return store->query(arg1->getValue(),
                          rightPredicateFactory->getPredicate(arg2),
                          &resultBuilder);
    } else if (arg2->isKnown()) {
      return store->query(leftPredicateFactory->getPredicate(arg1),
                          arg2->getValue(), &resultBuilder);
    } else {
      return store->query(leftProvider->getValuesOfType(arg1->getType()),
                          rightPredicateFactory->getPredicate(arg2),
                          &resultBuilder);
    }
  }
};
