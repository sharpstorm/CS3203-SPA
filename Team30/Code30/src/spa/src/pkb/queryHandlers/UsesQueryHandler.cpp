#include <string>
#include "UsesQueryHandler.h"

using std::string;

UsesQueryHandler::UsesQueryHandler(
    const UsesStorage *store,
    const PredicateFactory *predicateFactory,
    const IStructureMappingProvider *structureProvider,
    const IEntityMappingProvider *entitiesProvider)
    : store(store),
      predicateFactory(predicateFactory),
      structureProvider(structureProvider),
      entitiesProvider(entitiesProvider) {}

bool UsesQueryHandler::validateArg1(StmtRef arg) const {
  return arg.type != StmtType::Read;
}

bool UsesQueryHandler::validateArg1(EntityRef arg) const {
  return arg.type == EntityType::Procedure;
}

bool UsesQueryHandler::validateArg2(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Variable;
}

QueryResult<int, string> UsesQueryHandler::queryUses(StmtRef arg1,
                                                     EntityRef arg2) const {
  if (!validateArg1(arg1) || !validateArg2(arg2)) {
    return QueryResult<int, string>();
  }
  if (arg1.isKnown()) {
    return store->query(arg1.lineNum,
                        predicateFactory->getPredicate(arg2));
  } else if (arg2.isKnown()) {
    return store->query(predicateFactory->getPredicate(arg1), arg2.name);
  } else {
    return store->query(structureProvider->getStatementsOfType(arg1.type),
                        predicateFactory->getPredicate(arg2));
  }
}

QueryResult<string, string> UsesQueryHandler::queryUses(EntityRef arg1,
                                                        EntityRef arg2) const {
  if (!validateArg1(arg1) || !validateArg2(arg2)) {
    return QueryResult<string, string>();
  }
  if (arg1.isKnown()) {
    return store->query<string>(
        structureProvider->getProcedureLines(arg1.name),
        predicateFactory->getPredicate(arg2),
        structureProvider->getStmtProcedureTransformer());
  } else if (arg2.isKnown()) {
    return store->query<string>(
        predicateFactory->getPredicate({StmtType::None, 0}),
        arg2.name,
        structureProvider->getStmtProcedureTransformer());
  } else {
    unordered_set<string> variables =
        entitiesProvider->getSymbolsOfType(EntityType::Variable);
    return store->query<string>(
        predicateFactory->getPredicate(
            {StmtType::None, 0}),
        variables,
        structureProvider->getStmtProcedureTransformer());
  }
}
