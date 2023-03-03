#include <memory>
#include "QueryTokenParser.h"
#include "../../errors/QPSParserSyntaxError.h"

QueryTokenParser::QueryTokenParser(ISourceExpressionParser* exprParser,
                                   vector<PQLToken>* tokens):
                                   tokens(tokens), contextProvider(exprParser){
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
  return state.getQueryBuilder()->build();
}
