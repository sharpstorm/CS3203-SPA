#include <string>
#include "ModifiesQueryHandler.h"

using std::string;

ModifiesQueryHandler::ModifiesQueryHandler(
    const ModifiesStorage *modifiesStorage,
    const ModifiesPStorage *modifiesPStorage,
    const PredicateFactory *predicateFactory,
    const IStructureMappingProvider *structureProvider,
    const IEntityMappingProvider *entitiesProvider)
    : modifiesStorage(modifiesStorage),
      modifiesPStorage(modifiesPStorage),
      predicateFactory(predicateFactory),
      structureProvider(structureProvider),
      entitiesProvider(entitiesProvider) {}

bool ModifiesQueryHandler::validateArg1(StmtRef arg) const {
  return arg.type != StmtType::Print;
}

bool ModifiesQueryHandler::validateArg1(EntityRef arg) const {
  return arg.type == EntityType::Procedure
      || (arg.type == EntityType::None && arg.isKnown());
}

bool ModifiesQueryHandler::validateArg2(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Variable;
}

QueryResult<int, string> ModifiesQueryHandler::queryModifies(
    StmtRef arg1, EntityRef arg2) const {
  if (!validateArg1(arg1) || !validateArg2(arg2)) {
    return QueryResult<int, string>();
  }
  if (arg1.isKnown()) {
    return modifiesStorage->query(arg1.lineNum,
                                  predicateFactory->getPredicate(arg2));
  } else if (arg2.isKnown()) {
    return modifiesStorage->query(predicateFactory->getPredicate(arg1),
                                  arg2.name);
  } else {
    return modifiesStorage->query(
        structureProvider->getValuesOfType(arg1.type),
        predicateFactory->getPredicate(arg2));
  }
}

QueryResult<string, string> ModifiesQueryHandler::queryModifies(
    EntityRef arg1, EntityRef arg2) const {
  if (!validateArg1(arg1) || !validateArg2(arg2)) {
    return QueryResult<string, string>();
  }
  if (arg1.isKnown()) {
    return modifiesPStorage->query(arg1.name,
                                   predicateFactory->getPredicate(arg2));
  } else if (arg2.isKnown()) {
    return modifiesPStorage->query(predicateFactory->getPredicate(arg1),
                                   arg2.name);
  } else {
    return modifiesPStorage->query(
        entitiesProvider->getValuesOfType(EntityType::Procedure),
        predicateFactory->getPredicate(arg2));
  }
}
