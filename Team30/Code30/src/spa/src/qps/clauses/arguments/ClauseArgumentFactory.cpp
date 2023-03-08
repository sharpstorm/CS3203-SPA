#include "ClauseArgumentFactory.h"

#include "StmtArgument.h"
#include "EntityArgument.h"
#include "SynonymArgument.h"
#include "WildcardArgument.h"

using std::make_unique;

ClauseArgumentPtr ClauseArgumentFactory::create(const int &stmt) {
  return make_unique<StmtArgument>(stmt);
}

ClauseArgumentPtr ClauseArgumentFactory::create(const string &ident) {
  return make_unique<EntityArgument>(ident);
}

ClauseArgumentPtr ClauseArgumentFactory::createWildcard() {
  return make_unique<WildcardArgument>();
}

ClauseArgumentPtr ClauseArgumentFactory::create(
    const PQLQuerySynonym &variable) {
  return make_unique<SynonymArgument>(variable);
}
