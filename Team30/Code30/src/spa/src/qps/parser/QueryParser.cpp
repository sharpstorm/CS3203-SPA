#include "QueryParser.h"
#include <vector>

using std::vector, std::unique_ptr;

QueryParser::QueryParser(ISourceExpressionParser *exprParser):
    exprParser(exprParser) {}

PQLQueryPtr QueryParser::parseQuery(string* query) {
  QueryLexerResult tokens = lexer.getTokenStream(query);
  QueryTokenParser tokenParser(exprParser, tokens.get());
  return tokenParser.build();
}
