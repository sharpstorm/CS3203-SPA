#include "CallsQueryHandler.h"

CallsQueryHandler::CallsQueryHandler(
    const CallsStorage *store, const PredicateFactory *predicateFactory,
    const IEntityMappingProvider *entityProvider)
    : store(store),
      predicateFactory(predicateFactory),
      entityProvider(entityProvider) {}

bool CallsQueryHandler::validateArg1(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Procedure;
}

bool CallsQueryHandler::validateArg2(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Procedure;
}

QueryResult<string, string> CallsQueryHandler::queryCalls(EntityRef e1,
                                                          EntityRef e2) const {
  if (!validateArg1(e1) || !validateArg2(e2)) {
    return QueryResult<string, string>();
  }
  if (e1.isKnown()) {
    return store->query(e1.name, predicateFactory->getPredicate(e2));
  } else if (e2.isKnown()) {
    return store->query(predicateFactory->getPredicate(e1), e2.name);
  } else {
    return store->query(
        entityProvider->getValuesOfType(EntityType::Procedure),
        predicateFactory->getPredicate(e2));
  }
}

QueryResult<string, string> CallsQueryHandler::queryCallsStar(
    EntityRef e1, EntityRef e2) const {
  if (!validateArg1(e1) || !validateArg2(e2)) {
    return QueryResult<string, string>();
  }
  if (e1.isKnown()) {
    return store->queryT(e1.name, predicateFactory->getPredicate(e2));
  } else if (e2.isKnown()) {
    return store->queryT(predicateFactory->getPredicate(e1), e2.name);
  } else {
    return store->queryT(
        entityProvider->getValuesOfType(EntityType::Procedure),
        predicateFactory->getPredicate(e2));
  }
}
