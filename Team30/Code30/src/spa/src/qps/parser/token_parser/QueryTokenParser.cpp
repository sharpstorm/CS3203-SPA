#include "QueryTokenParser.h"
#include "qps/parser/builder/QueryBuilder.h"

QueryTokenParser::QueryTokenParser(ISourceExpressionParser *exprParser,
                                   PQLTokenVector *tokens) :
    tokens(tokens),
    conditionalParser(exprParser) {
}

PQLQueryPtr QueryTokenParser::build() {
  QueryTokenParseState state(this->tokens);
  QueryBuilder builder;

  declarationParser.parse(&state, &builder);
  selectParser.parse(&state, &builder);
  conditionalParser.parse(&state, &builder);

  if (!state.isTokenStreamEnd()) {
    throw QPSParserSyntaxError(QPS_PARSER_ERR_UNEXPECTED);
  }

  return builder.build();
}
