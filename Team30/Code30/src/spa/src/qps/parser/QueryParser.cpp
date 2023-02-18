#include "QueryParser.h"
#include <vector>

using std::vector, std::unique_ptr;

unique_ptr<PQLQuery> QueryParser::parseQuery(string* query) {
  QueryLexerResult tokens = lexer.getTokenStream(query);
  QueryTokenParser tokenParser(tokens.get());
  return tokenParser.build();
}
