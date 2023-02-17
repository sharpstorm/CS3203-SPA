#include "PredicateFactory.h"

PredicateFactory::PredicateFactory(const IStructureMappingProvider *sp,
                                   const IEntityMappingProvider *ep)
    : structureProvider(sp), entityProvider(ep) {}

Predicate<int> PredicateFactory::getPredicate(StmtRef stmtRef) const {
  if (stmtRef.lineNum != 0) {
    return [stmtRef](int const s) { return s == stmtRef.lineNum; };
  } else if (stmtRef.type == StmtType::None) {
    return [](int const s) { return true; };
  }
  return [this, stmtRef](int const s) {
    return structureProvider->getStatementType(s) == stmtRef.type;
  };
}

Predicate<string> PredicateFactory::getPredicate(EntityRef entRef) const {
  if (entRef.isKnown()) {
    return [entRef](string const s) { return s == entRef.name; };
  } else if (entRef.type == EntityType::None) {
    return [](string const s) { return true; };
  }
  return [this, entRef](string const s) {
    return entityProvider->getTypeOfSymbol(s) == entRef.type;
  };
}
