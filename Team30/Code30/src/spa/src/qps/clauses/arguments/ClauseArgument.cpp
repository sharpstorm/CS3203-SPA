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

PQLSynonymName ClauseArgument::getName() {
  return "";
}

SynonymPredicate ClauseArgument::isStatement = [](PQLQuerySynonym syn) {
  return syn.isStatementType();
};