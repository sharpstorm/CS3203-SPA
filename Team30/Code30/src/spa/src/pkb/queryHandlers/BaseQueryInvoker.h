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
      IStorage<LeftValue, RightValue> *store, IRef<LeftValue, LeftType> *arg1,
      IRef<RightValue, RightType> *arg2) const {
    auto resultBuilder = QueryResultBuilder<LeftValue, RightValue>();

    // wildcard
    if (arg1->isWildcard() && arg2->isWildcard()) {
      return store->hasRelation(&resultBuilder);
    } else if (arg1->isKnown() && arg2->isWildcard()) {
      const auto arg1Values = unordered_set<LeftValue>({arg1->getValue()});
      return store->rightWildcardQuery(arg1Values, &resultBuilder);
    } else if (arg1->isWildcard() && arg2->isKnown()) {
      const auto arg2Values = unordered_set<RightValue>({arg2->getValue()});
      return store->leftWildcardQuery(arg2Values, &resultBuilder);
    } else if (arg1->isWildcard()) {
      return store->leftWildcardQuery(
          rightProvider->getValuesOfType(arg2->getType()), &resultBuilder);
    } else if (arg2->isWildcard()) {
      return store->rightWildcardQuery(
          leftProvider->getValuesOfType(arg1->getType()), &resultBuilder);
    }

    // set result field
    auto skipLeftResult = arg1->isKnown();
    auto skipRightResult = arg2->isKnown();
    if (skipLeftResult && skipRightResult) {
      resultBuilder.setIsEmpty();
    } else if (skipLeftResult && !skipRightResult) {
      resultBuilder.setRightVals();
    } else if (!skipLeftResult && skipRightResult) {
      resultBuilder.setLeftVals();
    } else {
      resultBuilder.setPairVals();
    }

    // query
    if (arg1->isKnown() && arg2->isKnown()) {
      return store->query(arg1->getValue(), arg2->getValue(), &resultBuilder);
    } else if (arg1->isKnown()) {
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
