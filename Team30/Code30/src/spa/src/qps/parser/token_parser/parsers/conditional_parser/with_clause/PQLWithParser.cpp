#include <memory>
#include <utility>

#include "PQLWithParser.h"
#include "qps/clauses/WithClause.h"
#include "qps/clauses/arguments/WithArgument.h"
#include "qps/common/constraint/Constraint.h"
#include "qps/common/constraint/ConstantConstraint.h"
#include "qps/common/constraint/OverrideConstraint.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"
#include "qps/common/constraint/SynonymConstraint.h"

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
        std::move(left), std::move(right), builder);
    if (constraint != nullptr) {
      builder->addConstraint(constraint);
    }
  }
}

ConstraintSPtr PQLWithParser::parseConstraint(
    WithArgumentPtr left, WithArgumentPtr right, QueryBuilder* builder) {
  if (!left->isSyn() && !right->isSyn()) {
    return handleConstant(std::move(left), std::move(right));
  } else if (left->isSyn() && right->isSyn()) {
    return handleTwoSyns(std::move(left), std::move(right), builder);
  } else {
    // Cat 2
    return handleOverride(std::move(left), std::move(right), builder);
  }
}

ConstraintSPtr PQLWithParser::handleConstant(WithArgumentPtr left,
                                             WithArgumentPtr right) {
  return make_shared<ConstantConstraint>(
      std::move(left), std::move(right));
}

ConstraintSPtr PQLWithParser::handleOverride(WithArgumentPtr left,
                                             WithArgumentPtr right,
                                             QueryBuilder* builder) {
  bool isLeftSyn = left->isSyn();
  WithArgumentPtr synArg = isLeftSyn ? std::move(left) : std::move(right);
  WithArgumentPtr constArg = isLeftSyn ? std::move(right) : std::move(left);

  if (isNonDefaultCase(synArg->getAttrSyn())) {
    addWithSelectClause(builder, synArg->getAttrSyn(),
                        constArg->getIdentValue());
    return nullptr;
  }

  return parseOverrideConstraint(std::move(synArg), std::move(constArg));
}

ConstraintSPtr PQLWithParser::handleTwoSyns(WithArgumentPtr left,
                                            WithArgumentPtr right,
                                            QueryBuilder *builder) {
  if (left->getSynType() == right->getSynType()) {
    // TODO(sharpstorm): Same Syn Handling
    return nullptr;
  }

  // Different Synonym Types
  WithClausePtr withClause = make_unique<WithClause>(std::move(left),
                                                     std::move(right));
  builder->addWith(std::move(withClause));
  return nullptr;
}

ConstraintSPtr PQLWithParser::parseOverrideConstraint(
    WithArgumentPtr synArg, WithArgumentPtr staticArg) {
  if (staticArg->doesReturnInteger()) {
    return make_shared<OverrideConstraint>(
        synArg->getAttrSyn(), staticArg->getIntValue());
  } else {
    return make_shared<OverrideConstraint>(
        synArg->getAttrSyn(), staticArg->getIdentValue());
  }
}

bool PQLWithParser::isNonDefaultCase(AttributedSynonym attrSyn) {
  PQLSynonymType synType = attrSyn.getType();
  PQLSynonymAttribute synAttr = attrSyn.getAttribute();

  if (synAttr == VAR_NAME) {
    return synType == PQL_SYN_TYPE_READ || synType == PQL_SYN_TYPE_PRINT;
  } else if (synAttr == PROC_NAME) {
    return synType == PQL_SYN_TYPE_CALL;
  }

  return false;
}

void PQLWithParser::addWithSelectClause(QueryBuilder* builder,
                                        AttributedSynonym attrSyn,
                                        string identValue) {
  WithSelectClausePtr withSelect = make_unique<WithSelectClause>(
      attrSyn, identValue);
  builder->addWithSelect(std::move(withSelect));
}

