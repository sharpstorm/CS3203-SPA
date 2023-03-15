#include <string>
#include "UsesQueryHandler.h"

using std::string;

UsesQueryHandler::UsesQueryHandler(
    const UsesStorage *usesStorage,
    const UsesPStorage *usesPStorage,
    const PredicateFactory *predicateFactory,
    const IStructureMappingProvider *structureProvider,
    const IEntityMappingProvider *entitiesProvider)
    : usesStorage(usesStorage),
      usesPStorage(usesPStorage),
      predicateFactory(predicateFactory),
      structureProvider(structureProvider),
      entitiesProvider(entitiesProvider) {}

bool UsesQueryHandler::validateArg1(StmtRef arg) const {
  return arg.type != StmtType::Read;
}

bool UsesQueryHandler::validateArg1(EntityRef arg) const {
  return arg.type == EntityType::Procedure
      || (arg.type == EntityType::None && arg.isKnown());
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
    return usesStorage->query(arg1.lineNum,
                              predicateFactory->getPredicate(arg2));
  } else if (arg2.isKnown()) {
    return usesStorage->query(predicateFactory->getPredicate(arg1), arg2.name);
  } else {
    return usesStorage->query(structureProvider->getValuesOfType(arg1.type),
                              predicateFactory->getPredicate(arg2));
  }
}

QueryResult<string, string> UsesQueryHandler::queryUses(EntityRef arg1,
                                                        EntityRef arg2) const {
  if (!validateArg1(arg1) || !validateArg2(arg2)) {
    return QueryResult<string, string>();
  }
  if (arg1.isKnown()) {
    return usesPStorage->query(arg1.name,
                               predicateFactory->getPredicate(arg2));
  } else if (arg2.isKnown()) {
    return usesPStorage->query(predicateFactory->getPredicate(arg1), arg2.name);
  } else {
    return usesPStorage->query(
        entitiesProvider->getValuesOfType(EntityType::Procedure),
        predicateFactory->getPredicate(arg2));
  }
}
