#include "WhilePatternQueryHandler.h"

WhilePatternQueryHandler::WhilePatternQueryHandler(const WhilePatternStorage *store,
                                                   const PredicateFactory *factory,
                                                   const IStructureMappingProvider *provider)
    : store(store), predicateFactory(factory), structureProvider(provider) {}

bool WhilePatternQueryHandler::validateArg1(StmtRef arg) const {
  return arg.type == StmtType::While
      || (arg.type == StmtType::None && arg.isKnown());
}

bool WhilePatternQueryHandler::validateArg2(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Variable;
}

QueryResult<int, string>
WhilePatternQueryHandler::queryWhilePattern(StmtRef arg1,
                                            EntityRef arg2) const {
  if (!validateArg1(arg1) || !validateArg2(arg2)) {
    return QueryResult<int, string>();
  }

  if (arg1.isKnown()) {
    return store->query(arg1.lineNum, predicateFactory->getPredicate(arg2));
  } else if (arg2.isKnown()) {
    return store->query(predicateFactory->getPredicate(arg1), arg2.name);
  } else {
    return store->query(structureProvider->getStatementsOfType(arg1.type),
                        predicateFactory->getPredicate(arg2));
  }
}
