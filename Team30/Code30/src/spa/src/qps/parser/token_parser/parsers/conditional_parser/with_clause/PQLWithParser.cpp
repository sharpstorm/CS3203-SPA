#include <memory>
#include <utility>

#include "PQLWithParser.h"
#include "qps/clauses/WithClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"
#include "qps/clauses/arguments/WithArgument.h"

using std::make_unique;

void PQLWithParser::parse(QueryTokenParseState *parserState,
                          QueryBuilder *builder) {
  parserState->expect(PQL_TOKEN_WITH);
  unique_ptr<PQLToken> dummyAnd = make_unique<PQLToken>(PQL_TOKEN_AND);
  PQLToken* andToken = dummyAnd.get();

  while (andToken != nullptr) {
    WithClausePtr withClause = parseWithClause(parserState, builder);
    if (withClause == nullptr) {
      continue;
    } else {
      builder->addWith(std::move(withClause));
    }

    andToken = parserState->tryExpect(PQL_TOKEN_AND);
  }
}

WithClausePtr PQLWithParser::parseWithClause(QueryTokenParseState *parserState,
                                    QueryBuilder *builder) {
  // Expect either an integer, string or [syn, period, attrName]
  WithArgumentPtr left =
      PQLAttributeRefExtractor::extract(parserState, builder);

  if (left == nullptr) {
    return nullptr;
  }

  // Expect an equals
  parserState->expect(PQL_TOKEN_EQUALS);

  // Expect either an integer, string or [syn, period, attrName]
  WithArgumentPtr right =
      PQLAttributeRefExtractor::extract(parserState, builder);

  if (right == nullptr) {
    return nullptr;
  }

  // Create the clause here
  return make_unique<WithClause>(std::move(left), std::move(right));
}
