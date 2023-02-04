#include "PredicateFactory.h"

PredicateFactory::PredicateFactory(const StructureMappingProvider* p)
    : provider(p) {}

Predicate<int> PredicateFactory::getPredicate(StmtRef stmtRef) const {
  if (stmtRef.lineNum != 0) {
    return [stmtRef](int const s) { return s == stmtRef.lineNum; };
  }
  return [this, stmtRef](int const s) {
    return provider->isStatementOfType(s, stmtRef.type);
  };
}
