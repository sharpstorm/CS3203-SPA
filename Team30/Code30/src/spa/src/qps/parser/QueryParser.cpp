#include "QueryParser.h"

QueryParser::QueryParser(ISourceExpressionParser *exprParser) :
    exprParser(exprParser) {}

PQLQueryPtr QueryParser::parseQuery(const PQLQueryString *query) {
  QueryLexerPtr lexer = lexerFactory.makeLexer(query);
  PQLTokenStreamPtr tokens = lexer->getTokenStream();
  QueryTokenParser tokenParser(exprParser, tokens.get());
  return tokenParser.build();
}
