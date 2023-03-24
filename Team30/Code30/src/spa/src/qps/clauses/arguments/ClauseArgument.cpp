#include <string>

#include "ClauseArgument.h"

using std::to_string;

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

bool ClauseArgument::isStatement(PQLQuerySynonym syn) {
  return syn.isStatementType();
}
