#include <memory>
#include <utility>

#include "PQLWithParser.h"
#include "qps/clauses/WithClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"
#include "qps/clauses/arguments/WithArgument.h"

using std::make_unique, std::make_shared;

void PQLWithParser::parse(QueryTokenParseState *parserState,
                          QueryBuilder *builder) {
  parserState->expect(PQL_TOKEN_WITH);
  unique_ptr<PQLToken> dummyAnd = make_unique<PQLToken>(PQL_TOKEN_AND);
  PQLToken* andToken = dummyAnd.get();

  while (andToken != nullptr) {
    parseWithClause(parserState, builder);
//    WithClausePtr withClause = parseWithClause(parserState, builder);
//    if (withClause == nullptr) {
//      continue;
//    } else {
//      builder->addWith(std::move(withClause));
//    }

    andToken = parserState->tryExpect(PQL_TOKEN_AND);
  }
}

//WithClausePtr PQLWithParser::parseWithClause(QueryTokenParseState *parserState,
//                                    QueryBuilder *builder) {
//  WithArgumentPtr left =
//      PQLAttributeRefExtractor::extract(parserState, builder);
//
//  if (left == nullptr) {
//    return nullptr;
//  }
//
//  parserState->expect(PQL_TOKEN_EQUALS);
//
//  WithArgumentPtr right =
//      PQLAttributeRefExtractor::extract(parserState, builder);
//
//  if (right == nullptr) {
//    return nullptr;
//  }
//
//  return make_unique<WithClause>(std::move(left), std::move(right));
//}

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
    return;
  }

  ConstraintSPtr constraint =
      make_shared<Constraint>(std::move(left), std::move(right));
  builder->addConstraint(constraint);
}
