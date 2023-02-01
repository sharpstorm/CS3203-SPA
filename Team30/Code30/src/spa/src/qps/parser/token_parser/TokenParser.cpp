#include <memory>
#include "TokenParser.h"
#include "../../errors/QPSParserError.h"

TokenParser::TokenParser(vector<PQLToken> tokens) {
  this->tokens = tokens;
}

unique_ptr<PQLQuery> TokenParser::build() {
  TokenParseState state(&this->tokens);

  while (!state.isEnd()) {
    unique_ptr<IPQLContext> context = contextProvider
        .getContext(state.getCurrentToken());
    if (context == nullptr) {
      throw QPSParserError("Unexpected token");
    }
    state.advanceToken();
    context->parse(&state);
  }

  return state.getQueryBuilder()->build();
}
