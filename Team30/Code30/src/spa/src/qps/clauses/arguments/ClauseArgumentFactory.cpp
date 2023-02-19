#include "ClauseArgumentFactory.h"

#include "StmtArgument.h"
#include "EntityArgument.h"
#include "SynonymArgument.h"
#include "WildcardArgument.h"

using std::make_unique;

ClauseArgumentPtr ClauseArgumentFactory::create(int stmt) {
  return make_unique<StmtArgument>(stmt);
}

ClauseArgumentPtr ClauseArgumentFactory::create(string ident) {
  return make_unique<EntityArgument>(ident);
}

ClauseArgumentPtr ClauseArgumentFactory::createWildcard() {
  return make_unique<WildcardArgument>();
}

ClauseArgumentPtr ClauseArgumentFactory::create(PQLQuerySynonym variable) {
  return make_unique<SynonymArgument>(variable);
}
