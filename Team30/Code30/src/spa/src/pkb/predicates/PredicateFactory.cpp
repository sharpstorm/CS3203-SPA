#include "PredicateFactory.h"

PredicateFactory::PredicateFactory(const StructureMappingProvider* p)
    : provider(p) {}

IsStmtOfTypePredicate PredicateFactory::getIsStmtOfTypePredicate(
    StmtType stmtType) const {
  return IsStmtOfTypePredicate(provider, stmtType);
}
