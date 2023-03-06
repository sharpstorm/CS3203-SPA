#include <memory>
#include "QueryTokenParser.h"
#include "qps/parser/builder/QueryBuilder.h"

QueryTokenParser::QueryTokenParser(ISourceExpressionParser* exprParser,
                                   vector<PQLToken>* tokens):
                                   tokens(tokens),
                                   conditionalParser(exprParser) {
}

unique_ptr<PQLQuery> QueryTokenParser::build() {
  QueryTokenParseState state(this->tokens);
  QueryBuilder builder;

  declarationParser.parse(&state, &builder);
  selectParser.parse(&state, &builder);
  conditionalParser.parse(&state, &builder);

  return builder.build();
}
