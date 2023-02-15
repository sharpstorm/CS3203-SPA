#include <memory>
#include "QueryTokenParser.h"
#include "../../errors/QPSParserSyntaxError.h"
#include "qps/errors/QPSParserSemanticError.h"

QueryTokenParser::QueryTokenParser(vector<PQLToken>* tokens) {
  this->tokens = tokens;
}

unique_ptr<PQLQuery> QueryTokenParser::build() {
  QueryTokenParseState state(this->tokens);

  while (!state.isTokenStreamEnd()) {
    IPQLContext* context = contextProvider.getContext(state.getCurrentToken());
    if (context == nullptr) {
      throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
    }
    state.advanceToken();
    context->parse(&state);
  }

  state.advanceStage(TOKEN_PARSE_STAGE_PARSE_END);
  if (state.hasSemanticError()) {
    throw QPSParserSemanticError(state.getSemanticError().c_str());
  }
  return state.getQueryBuilder()->build();
}
