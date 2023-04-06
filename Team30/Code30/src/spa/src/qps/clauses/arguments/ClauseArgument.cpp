#include "ClauseArgument.h"

bool ClauseArgument::synonymSatisfies(SynonymPredicate predicate) const {
  return true;
}

bool ClauseArgument::isNamed() const {
  return false;
}

bool ClauseArgument::isWildcard() const {
  return false;
}

bool ClauseArgument::isConstant() const {
  return false;
}

PQLSynonymName ClauseArgument::getName() const {
  return "";
}

ComplexityScore ClauseArgument::getSynComplexity() const {
  return 0;
}

bool ClauseArgument::isStatement(const PQLQuerySynonym &syn) {
  return syn.isStatementType();
}
