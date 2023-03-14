#include <memory>
#include <utility>

#include "PQLWithParser.h"
#include "qps/clauses/WithClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"
#include "qps/clauses/arguments/WithArgument.h"
#include "qps/common/constraint/Constraint.h"
#include "qps/common/constraint/ConstantConstraint.h"
#include "qps/common/constraint/OverrideConstraint.h"

using std::make_unique, std::make_shared;

void PQLWithParser::parse(QueryTokenParseState *parserState,
                          QueryBuilder *builder) {
  parserState->expect(PQL_TOKEN_WITH);
  unique_ptr<PQLToken> dummyAnd = make_unique<PQLToken>(PQL_TOKEN_AND);
  PQLToken* andToken = dummyAnd.get();

  while (andToken != nullptr) {
    parseWithClause(parserState, builder);
    andToken = parserState->tryExpect(PQL_TOKEN_AND);
  }
}

void PQLWithParser::parseWithClause(QueryTokenParseState *parserState,
                                             QueryBuilder *builder) {
  WithArgumentPtr left =
      PQLAttributeRefExtractor::extract(parserState, builder);

  if (left == nullptr) {
    return;
  }

  parserState->expect(PQL_TOKEN_EQUALS);

  WithArgumentPtr right =
      PQLAttributeRefExtractor::extract(parserState, builder);

  if (right == nullptr) {
    return;
  }

  if (left->isSyn() && right->isSyn() &&
      left->getSynType() != right->getSynType()) {
    WithClausePtr withClause =
        make_unique<WithClause>(std::move(left), std::move(right));
    builder->addWith(std::move(withClause));
  } else {
    ConstraintSPtr constraint = parseConstraint(
        std::move(left), std::move(right));
    builder->addConstraint(constraint);
  }
}


ConstraintSPtr PQLWithParser::parseConstraint(
    WithArgumentPtr left, WithArgumentPtr right) {
  ConstraintSPtr constraint;
  if (!left->isSyn() && !right->isSyn()) {
    constraint = make_shared<ConstantConstraint>(
        std::move(left), std::move(right));
  } else if (left->isSyn() && right->isSyn()) {
    // TODO(KwanHW): Cat 3 here
  } else {
    // Cat 2
    if (left->isSyn()) {
      constraint = parseOverrideConstraint(std::move(left), std::move(right));
    } else {
      constraint = parseOverrideConstraint(std::move(right), std::move(left));
    }
  }

  return constraint;
}

ConstraintSPtr PQLWithParser::parseOverrideConstraint(
    WithArgumentPtr synArg, WithArgumentPtr staticArg) {
  if (staticArg->doesReturnInteger()) {
    OverrideTransformer trans(staticArg->getIntValue());
    return make_shared<OverrideConstraint>(synArg->getAttrSyn(), trans);
  } else {
    OverrideTransformer trans(staticArg->getIdentValue());
    return make_shared<OverrideConstraint>(synArg->getAttrSyn(), trans);
  }
}
