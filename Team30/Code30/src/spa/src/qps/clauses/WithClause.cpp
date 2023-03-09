#include <utility>

#include "WithClause.h"

WithClause::WithClause(ClauseArgumentPtr left, ClauseArgumentPtr right) :
    leftArg(std::move(left)), rightArg(std::move(right)) {}

PQLQueryResult *WithClause::evaluateOn(PkbQueryHandler *pkbQueryHandler) {
  return nullptr;
}

bool WithClause::validateArgTypes(VariableTable *variables) {
  if (leftArg->isNamed() && !leftArg->hasAttribute()) {
    return false;
  }

  if (rightArg->isNamed() && !rightArg->hasAttribute()) {
    return false;
  }

  return true;
}

SynonymList WithClause::getUsedSynonyms() {
  SynonymList result;
  if (leftArg->isNamed()) {
   result.push_back(leftArg->getName());
  }

  if (rightArg->isNamed()) {
    result.push_back(rightArg->getName());
  }

  return result;
}
