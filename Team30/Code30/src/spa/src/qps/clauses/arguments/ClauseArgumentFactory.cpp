#include "ClauseArgumentFactory.h"

#include <memory>

#include "StmtArgument.h"
#include "EntityArgument.h"
#include "SynonymArgument.h"
#include "WildcardArgument.h"

using std::make_unique;

ClauseArgumentPtr ClauseArgumentFactory::create(const StmtValue &stmt) {
  return make_unique<StmtArgument>(stmt);
}

ClauseArgumentPtr ClauseArgumentFactory::create(const EntityValue &ident) {
  return make_unique<EntityArgument>(ident);
}

ClauseArgumentPtr ClauseArgumentFactory::createWildcard() {
  return make_unique<WildcardArgument>();
}

ClauseArgumentPtr ClauseArgumentFactory::create(
    const PQLQuerySynonymProxy &synProxy) {
  return make_unique<SynonymArgument>(synProxy);
}
