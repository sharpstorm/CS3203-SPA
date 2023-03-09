#include "IfPatternQueryHandler.h"

IfPatternQueryHandler::IfPatternQueryHandler(const IfPatternStorage *store)
    : store(store) {}

bool IfPatternQueryHandler::validateArg(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Variable;
}

QueryResult<int,
            string> IfPatternQueryHandler::queryIfPattern(EntityRef arg) const {
  if (!validateArg(arg)) {
    return QueryResult<int, string>();
  }
  if (arg.isKnown()) {
    return store->queryByValue(arg.name);
  }
  return store->queryAll();
}
