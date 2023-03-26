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

  if (!left->isSyn() && !right->isSyn()) {
    handleConstant(std::move(left), std::move(right), builder);
  } else if (left->isSyn() && right->isSyn()) {
    handleTwoSyns(std::move(left), std::move(right), builder);
  } else {
    handleOverride(std::move(left), std::move(right), builder);
  }
}

void PQLWithParser::handleConstant(WithArgumentPtr left,
                                   WithArgumentPtr right,
                                   QueryBuilder* builder) {
  ConstraintPtr constraint = make_unique<ConstantConstraint>(std::move(left),
                                                             std::move(right));
  builder->addConstraint(std::move(constraint));
}

void PQLWithParser::handleOverride(WithArgumentPtr left,
                                   WithArgumentPtr right,
                                   QueryBuilder* builder) {
  bool isLeftSyn = left->isSyn();
  WithArgumentPtr synArg = isLeftSyn ? std::move(left) : std::move(right);
  WithArgumentPtr constArg = isLeftSyn ? std::move(right) : std::move(left);

  if (!synArg->isDefaultAttribute()) {
    WithSelectClausePtr withSelect = make_unique<WithSelectClause>(
        synArg->toAttrSyn(), constArg->toConstIdent());
    builder->addWithSelect(std::move(withSelect));
    return;
  }

  ConstraintPtr constraint = parseOverrideConstraint(std::move(synArg),
                                                     std::move(constArg));
  builder->addConstraint(std::move(constraint));
}

void PQLWithParser::handleTwoSyns(WithArgumentPtr left,
                                  WithArgumentPtr right,
                                  QueryBuilder *builder) {
  bool isSameType = left->isSameSynTypeAs(right.get());
  bool isBothStmt = left->isSynStatement() && right->isSynStatement();
  bool isBothDefault = left->isDefaultAttribute()
      && right->isDefaultAttribute();

  if (isSameType || (isBothStmt && isBothDefault)) {
    ConstraintPtr constraint = make_unique<SynonymConstraint>(
        left->getSynName(),
        right->getSynName());
    builder->addConstraint(std::move(constraint));
    return;
  }

  WithClausePtr withSelect = make_unique<WithClause>(
      std::move(left->toAttrSyn()), std::move(right->toAttrSyn()));
  builder->addWith(std::move(withSelect));
}

ConstraintPtr PQLWithParser::parseOverrideConstraint(
    WithArgumentPtr synArg, WithArgumentPtr staticArg) {
  if (staticArg->doesReturnInteger()) {
    return make_unique<OverrideConstraint>(
        synArg->toAttrSyn(), staticArg->toConstInt());
  } else {
    return make_unique<OverrideConstraint>(
        synArg->toAttrSyn(), staticArg->toConstIdent());
  }
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
