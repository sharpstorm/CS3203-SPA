#include "StmtPredicateFactory.h"

StmtPredicateFactory::StmtPredicateFactory(IStructureMappingProvider *provider)
    : AbstractPredicateFactory<StmtValue, StmtType>(),
    provider(provider) {}

Predicate<StmtValue> StmtPredicateFactory::handleType(
    IRef<StmtValue,
         StmtType> *ref) const {
  return [ref, this](StmtValue const s) {
    return provider->isValueOfType(ref->getType(), s);
  };
}
