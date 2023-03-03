#include "PredicateFactory.h"

PredicateFactory::PredicateFactory(const IStructureMappingProvider *sp,
                                   const IEntityMappingProvider *ep)
    : structureProvider(sp), entityProvider(ep) {}

Predicate<int> PredicateFactory::getPredicate(StmtRef stmtRef) const {
  if (stmtRef.isKnown()) {
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
  } else {
    // a table column cannot store different entity types
    return [](string const s) { return true; };
  }
}
