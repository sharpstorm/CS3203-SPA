#include "ClauseArgument.h"

bool ClauseArgument::isSynonymCalled(PQLSynonymName name) {
  return false;
}

bool ClauseArgument::synonymSatisfies(SynonymPredicate predicate) {
  return true;
}

bool ClauseArgument::isNamed() {
  return false;
}

bool ClauseArgument::isWildcard() {
  return false;
}

void ClauseArgument::invokeWithName(function<void(PQLSynonymName)> consumer) {
}

SynonymPredicate ClauseArgument::isStatement = [](PQLQuerySynonym syn) {
  return syn.isStatementType();
};
