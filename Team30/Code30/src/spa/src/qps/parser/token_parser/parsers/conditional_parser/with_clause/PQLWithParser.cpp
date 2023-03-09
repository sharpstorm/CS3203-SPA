#include <memory>
#include <utility>

#include "PQLWithParser.h"
#include "qps/clauses/WithClause.h"
#include "qps/parser/token_parser/ref_extractor/PQLAttributeRefExtractor.h"

using std::make_unique;

void PQLWithParser::parse(QueryTokenParseState *parserState,
                          QueryBuilder *builder) {
  parserState->expect(PQL_TOKEN_WITH);
  WithClausePtr withClause = parseWithClause(parserState, builder);
}

WithClausePtr PQLWithParser::parseWithClause(QueryTokenParseState *parserState,
                                    QueryBuilder *builder) {
  // Expect either an integer, string or [syn, period, attrName]
  ClauseArgumentPtr left =
      PQLAttributeRefExtractor::extract(parserState, builder);

  // Expect an equals
  parserState->expect(PQL_TOKEN_EQUALS);

  // Expect either an integer, string or [syn, period, attrName]
  ClauseArgumentPtr right =
      PQLAttributeRefExtractor::extract(parserState, builder);

  // Create the clause here
  return make_unique<WithClause>(std::move(left), std::move(right));
}
