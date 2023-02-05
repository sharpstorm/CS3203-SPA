#include "QueryParser.h"
#include <vector>
#include "PQLToken.h"

using std::vector, std::unique_ptr;

unique_ptr<PQLQuery> QueryParser::parseQuery(string* query) {
  vector<PQLToken> tokens = lexer.getTokenStream(query);
  QueryTokenParser tokenParser(tokens);
  return tokenParser.build();
}
