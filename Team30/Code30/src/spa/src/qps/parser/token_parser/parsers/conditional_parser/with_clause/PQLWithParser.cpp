#include <memory>
#include <utility>

#include "PQLWithParser.h"
#include "qps/clauses/WithClause.h"
#include "qps/clauses/arguments/WithArgument.h"
#include "qps/constraints/Constraint.h"
#include "qps/constraints/ConstantConstraint.h"
#include "qps/constraints/OverrideConstraint.h"
#include "qps/constraints/SynonymConstraint.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"

using std::make_unique;

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
  WithArgumentPtr left = parseWithArg(parserState, builder);
  parserState->expect(PQL_TOKEN_EQUALS);
  WithArgumentPtr right = parseWithArg(parserState, builder);

  if (left == nullptr || right == nullptr) {
    return;
  }

  if (isWithClause(left.get(), right.get())) {
    WithClausePtr withClause = make_unique<WithClause>(left->getAttrSyn(),
                                                       right->getAttrSyn());
    builder->addWith(std::move(withClause));
  } else {
    ConstraintPtr constraint = parseConstraint(std::move(left),
                                               std::move(right), builder);
    if (constraint != nullptr) {
      builder->addConstraint(std::move(constraint));
    }
  }
}

ConstraintPtr PQLWithParser::parseConstraint(
    WithArgumentPtr left, WithArgumentPtr right, QueryBuilder* builder) {
  if (!left->isSyn() && !right->isSyn()) {
    return handleConstant(std::move(left), std::move(right));
  } else if (left->isSyn() && right->isSyn()) {
    return handleSameSyn(std::move(left), std::move(right));
  } else {
    return handleOverride(std::move(left), std::move(right), builder);
  }
}

ConstraintPtr PQLWithParser::handleConstant(WithArgumentPtr left,
                                            WithArgumentPtr right) {
  return make_unique<ConstantConstraint>(std::move(left), std::move(right));
}

ConstraintPtr PQLWithParser::handleOverride(WithArgumentPtr left,
                                            WithArgumentPtr right,
                                            QueryBuilder* builder) {
  bool isLeftSyn = left->isSyn();
  WithArgumentPtr synArg = isLeftSyn ? std::move(left) : std::move(right);
  WithArgumentPtr constArg = isLeftSyn ? std::move(right) : std::move(left);

  if (!synArg->isDefaultAttribute()) {
    addWithSelectClause(builder, synArg->getAttrSyn(),
                        constArg->getIdentValue());
    return nullptr;
  }

  return parseOverrideConstraint(std::move(synArg), std::move(constArg));
}

ConstraintPtr PQLWithParser::handleSameSyn(WithArgumentPtr left,
                                           WithArgumentPtr right) {
  return make_unique<SynonymConstraint>(left->getSynName(),
                                        right->getSynName());
}

ConstraintPtr PQLWithParser::parseOverrideConstraint(
    WithArgumentPtr synArg, WithArgumentPtr staticArg) {
  if (staticArg->doesReturnInteger()) {
    return make_unique<OverrideConstraint>(
        synArg->getAttrSyn(), staticArg->getIntValue());
  } else {
    return make_unique<OverrideConstraint>(
        synArg->getAttrSyn(), staticArg->getIdentValue());
  }
}

void PQLWithParser::addWithSelectClause(QueryBuilder* builder,
                                        const AttributedSynonym &attrSyn,
                                        const string &identValue) {
  WithSelectClausePtr withSelect = make_unique<WithSelectClause>(
      attrSyn, identValue);
  builder->addWithSelect(std::move(withSelect));
}

WithArgumentPtr PQLWithParser::parseWithArg(QueryTokenParseState* state,
                                            QueryBuilder* builder) {
  PQLToken* token = state->tryExpect(PQL_TOKEN_STRING_LITERAL,
                                     PQL_TOKEN_INTEGER);
  if (token != nullptr) {
    return processConstant(token);
  }

  AttributedSynonymPtr attrSyn =
      PQLAttributeRefExtractor::extract(state, builder);
  if (attrSyn == nullptr) {
    return nullptr;
  }

  if (!attrSyn->hasAttribute()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  if (!attrSyn->validateAttribute()) {
    builder->setError(QPS_PARSER_ERR_INVALID_ATTRIBUTE);
    return nullptr;
  }

  return make_unique<WithArgument>(std::move(attrSyn));
}

WithArgumentPtr PQLWithParser::processConstant(PQLToken *token) {
  if (token->isType(PQL_TOKEN_STRING_LITERAL)) {
    return make_unique<WithArgument>(token->getData());
  } else if (token->isType(PQL_TOKEN_INTEGER)) {
    int intVal = stoi(token->getData());
    return make_unique<WithArgument>(intVal);
  }
  return nullptr;
}

bool PQLWithParser::isWithClause(WithArgument *left, WithArgument *right) {
  if (!left->isSyn() || !right->isSyn()) {
    return false;
  }

  bool isDiffType = left->getSynType() != right->getSynType();
  return isDiffType
      || !left->isDefaultAttribute() || !right->isDefaultAttribute();
}
