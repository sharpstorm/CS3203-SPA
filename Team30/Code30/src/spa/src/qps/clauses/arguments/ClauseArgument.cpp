#include "ClauseArgument.h"

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

bool ClauseArgument::canSubstitute(OverrideTable *table) {
  if (isNamed()) {
    return false;
  }

  return table->find(getName()) != table->end();
}
