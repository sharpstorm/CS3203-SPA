/* * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur */

#pragma once
#include <unordered_set>

#include "common/Types.h"
#include "pkb/predicates/AbstractPredicateFactory.h"
#include "pkb/queryHandlers/QueryResultBuilder.h"
#include "pkb/storage/RelationTableManager.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"
#include "pkb/storage/interfaces/IProvider.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

using std::unordered_set;

template <typename LeftValue, typename LeftType, typename RightValue,
          typename RightType>
class BaseQueryInvoker {
 private:
  IProvider<LeftValue, LeftType> *leftProvider;
  IProvider<RightValue, RightType> *rightProvider;
  AbstractPredicateFactory<LeftValue, LeftType> *leftPredicateFactory;
  AbstractPredicateFactory<RightValue, RightType> *rightPredicateFactory;

 public:
  BaseQueryInvoker(
      IProvider<LeftValue, LeftType> *leftProvider,
      IProvider<RightValue, RightType> *rightProvider,
      AbstractPredicateFactory<LeftValue, LeftType> *leftPredicateFactory,
      AbstractPredicateFactory<RightValue, RightType> *rightPredicateFactory)
      : leftProvider(leftProvider),
        rightProvider(rightProvider),
        leftPredicateFactory(leftPredicateFactory),
        rightPredicateFactory(rightPredicateFactory) {}

  QueryResultPtr<LeftValue, RightValue> query(
      IStorage<LeftValue, RightValue> *store,
      IRef<LeftValue, LeftType> *leftArg,
      IRef<RightValue, RightType> *rightArg) const {
    auto resultBuilder = QueryResultBuilder<LeftValue, RightValue>();

    // wildcard
    if (leftArg->isWildcard() && rightArg->isWildcard()) {
      return store->hasRelation(&resultBuilder);
    } else if (leftArg->isKnown() && rightArg->isWildcard()) {
      const auto arg1Values = unordered_set<LeftValue>({leftArg->getValue()});
      return store->rightWildcardQuery(arg1Values, &resultBuilder);
    } else if (leftArg->isWildcard() && rightArg->isKnown()) {
      const auto arg2Values = unordered_set<RightValue>({rightArg->getValue()});
      return store->leftWildcardQuery(arg2Values, &resultBuilder);
    } else if (leftArg->isWildcard()) {
      return store->leftWildcardQuery(
          rightProvider->getValuesOfType(rightArg->getType()), &resultBuilder);
    } else if (rightArg->isWildcard()) {
      return store->rightWildcardQuery(
          leftProvider->getValuesOfType(leftArg->getType()), &resultBuilder);
    }

    if (leftArg->isKnown() && rightArg->isKnown()) {
      return store->query(leftArg->getValue(), rightArg->getValue(),
                          &resultBuilder);
    } else if (leftArg->isKnown()) {
      resultBuilder.setRightVals();
      const auto arg1Values = unordered_set<LeftValue>({leftArg->getValue()});
      return store->query(arg1Values,
                          rightPredicateFactory->getPredicate(rightArg),
                          &resultBuilder);
    } else if (rightArg->isKnown()) {
      resultBuilder.setLeftVals();
      const auto arg2Values = unordered_set<RightValue>({rightArg->getValue()});
      return store->query(leftPredicateFactory->getPredicate(leftArg),
                          arg2Values, &resultBuilder);
    } else {
      resultBuilder.setPairVals();
      return store->query(leftProvider->getValuesOfType(leftArg->getType()),
                          rightPredicateFactory->getPredicate(rightArg),
                          &resultBuilder);
    }
  }
};
