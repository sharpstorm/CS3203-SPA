#include <utility>

#include "WithClause.h"
#include "qps/errors/QPSParserSemanticError.h"

WithClause::WithClause(WithArgumentPtr left, WithArgumentPtr right) :
    leftArg(std::move(left)), rightArg(std::move(right)) {}

PQLQueryResult *WithClause::evaluateOn(PkbQueryHandler *pkbQueryHandler) {
  return nullptr;
}

bool WithClause::validateArgTypes(VariableTable *variables) {
  if (leftArg->getIsSyn() && !leftArg->isAttributeValid()) {
    throw QPSParserSemanticError(QPS_PARSER_ERR_INVALID_ATTRIBUTE);
  }

  if (rightArg->getIsSyn() && !rightArg->isAttributeValid()) {
    throw QPSParserSemanticError(QPS_PARSER_ERR_INVALID_ATTRIBUTE);
  }

  bool leftRetInt = leftArg->doesReturnInteger();
  bool rightRetInt = rightArg->doesReturnInteger();

  // They both must return an int or a str
  return (leftRetInt && rightRetInt) || (!leftRetInt && !rightRetInt);
}

SynonymList WithClause::getUsedSynonyms() {
  SynonymList result;
  if (leftArg->getIsSyn()) {
    result.push_back(leftArg->getSynName());
  }

  if (rightArg->getIsSyn()) {
    result.push_back(rightArg->getSynName());
  }

  return result;
}
