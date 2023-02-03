#include <memory>
#include "TokenParser.h"
#include "../../errors/QPSParserError.h"
#include "../builder/QueryBuilderError.h"

TokenParser::TokenParser(vector<PQLToken> tokens) {
  this->tokens = tokens;
}

unique_ptr<PQLQuery> TokenParser::build() {
  TokenParseState state(&this->tokens);

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
