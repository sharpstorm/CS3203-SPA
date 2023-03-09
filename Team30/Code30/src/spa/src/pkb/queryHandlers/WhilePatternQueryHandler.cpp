#include "WhilePatternQueryHandler.h"

WhilePatternQueryHandler::WhilePatternQueryHandler(const WhilePatternStorage *store)
    : store(store) {}

bool WhilePatternQueryHandler::validateArg(EntityRef arg) const {
  return arg.type == EntityType::None || arg.type == EntityType::Variable;
}

QueryResult<int,
            string> WhilePatternQueryHandler::queryWhilePattern(EntityRef arg) const {
  if (!validateArg(arg)) {
    return QueryResult<int, string>();
  }
  if (arg.isKnown()) {
    return store->queryByValue(arg.name);
  }
  return store->queryAll();
}
