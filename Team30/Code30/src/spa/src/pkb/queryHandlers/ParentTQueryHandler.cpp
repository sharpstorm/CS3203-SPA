#include "ParentTQueryHandler.h"

#include <memory>
#include <unordered_set>

#include "ArgValidators.h"
using std::make_unique, std::unordered_set;

ParentTQueryHandler::ParentTQueryHandler(ParentTStorage *storage,
                                         IStructureMappingProvider *provider,
                                         StmtPredicateFactory *factory)
    : store(storage), provider(provider), factory(factory) {}

QueryResultPtr<StmtValue, StmtValue> ParentTQueryHandler::query(
    StmtRef *leftArg, StmtRef *rightArg) const {
  if (!parentLeftArgValidator(leftArg)) {
    return make_unique<QueryResult<StmtValue, StmtValue>>();
  }
  if (leftArg->isWildcard() && rightArg->isWildcard()) {
    return store->hasRelation();
  } else if (leftArg->isKnown() && rightArg->isWildcard()) {
    const auto leftArgValues = unordered_set<StmtValue>({leftArg->getValue()});
    return store->rightWildcardQuery(leftArgValues);
  } else if (leftArg->isWildcard() && rightArg->isKnown()) {
    const auto rightArgValues =
        unordered_set<StmtValue>({rightArg->getValue()});
    return store->leftWildcardQuery(rightArgValues);
  } else if (leftArg->isWildcard()) {
    return store->leftWildcardQuery(
        provider->getValuesOfType(rightArg->getType()));
  } else if (rightArg->isWildcard()) {
    return store->rightWildcardQuery(
        provider->getValuesOfType(leftArg->getType()));
  }

  if (leftArg->isKnown() && rightArg->isKnown()) {
    return store->query(leftArg->getValue(), rightArg->getValue());
  } else if (leftArg->isKnown() && !rightArg->isKnown()) {
    return store->query({leftArg->getValue()}, factory->getPredicate(rightArg));
  } else if (rightArg->isKnown()) {
    return store->query(factory->getPredicate(leftArg), rightArg->getValue());
  } else {
    return store->query(provider->getValuesOfType(leftArg->getType()),
                        factory->getPredicate(rightArg));
  }
}
