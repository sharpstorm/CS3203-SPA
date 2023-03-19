#include "PatternClause.h"

#include <utility>

PatternClause::PatternClause(
    const PQLQuerySynonymProxy &synonym,
    ClauseArgumentPtr leftArg,
    PQLSynonymType synType):
    synonym(synonym), leftArg(std::move(leftArg)), synType(synType) {}

SynonymList PatternClause::getUsedSynonyms() {
  SynonymList result{ synonym->getName() };
  if (leftArg->isNamed()) {
    result.push_back(leftArg->getName());
  }
  return result;
}

bool PatternClause::validateArgTypes(VariableTable *variables) {
  if (!synonym->isType(synType)) {
    return false;
  }

  return leftArg->synonymSatisfies(
      ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>);
}
