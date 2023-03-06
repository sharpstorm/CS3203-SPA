#pragma once

#include <string>

#include "IQueryParser.h"
#include "lexer/QueryLexer.h"
#include "token_parser/QueryTokenParser.h"
#include "common/parser/ISourceExpressionParser.h"

using std::string;

class QueryParser: public IQueryParser {
 public:
  explicit QueryParser(ISourceExpressionParser* exprParser);
  PQLQueryPtr parseQuery(string* query);

 private:
  QueryLexer lexer;
  ISourceExpressionParser* exprParser;
};
