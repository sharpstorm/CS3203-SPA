#include <memory>
#include "QueryTokenParser.h"
#include "../../errors/QPSParserError.h"
#include "../builder/QueryBuilderError.h"

QueryTokenParser::QueryTokenParser(vector<PQLToken> tokens) {
  this->tokens = tokens;
}

unique_ptr<PQLQuery> QueryTokenParser::build() {
  QueryTokenParseState state(&this->tokens);

  while (!state.isTokenStreamEnd()) {
    unique_ptr<IPQLContext> context = contextProvider
        .getContext(state.getCurrentToken());
    if (context == nullptr) {
      throw QPSParserError("Unexpected token");
    }
    state.advanceToken();

    try {
      context->parse(&state);
    } catch (const QueryBuilderError& err) {
      throw QPSParserError(err.message);
    }
  }

  state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END);
  return state.getQueryBuilder()->build();
}
