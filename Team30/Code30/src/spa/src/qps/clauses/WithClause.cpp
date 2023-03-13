#include <utility>

#include "WithClause.h"

WithClause::WithClause(WithArgumentPtr left, WithArgumentPtr right) :
    leftArg(std::move(left)), rightArg(std::move(right)) {}

PQLQueryResult *WithClause::evaluateOn(PkbQueryHandler *pkbQueryHandler) {
  PQLQueryResult* result = new PQLQueryResult();

  // Cat 4 -
  return result;
}

bool WithClause::validateArgTypes(VariableTable *variables) {
  if (!leftArg->isAttributeValid() || !rightArg->isAttributeValid()) {
    return false;
  }
  bool leftRetInt = leftArg->doesReturnInteger();
  bool rightRetInt = rightArg->doesReturnInteger();

  // They both must return an int or a str
  return leftRetInt == rightRetInt;
}

SynonymList WithClause::getUsedSynonyms() {
  SynonymList result;
  if (leftArg->isSyn()) {
    result.push_back(leftArg->getSynName());
  }

  if (rightArg->isSyn()) {
    result.push_back(rightArg->getSynName());
  }

  return result;
}
