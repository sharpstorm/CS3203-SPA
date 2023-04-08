#pragma once

#include "IQueryParser.h"
#include "lexer/QueryLexer.h"
#include "token_parser/QueryTokenParser.h"
#include "common/parser/ISourceExpressionParser.h"
#include "qps/parser/lexer/QueryLexerFactory.h"
#include "qps/common/QPSTypes.h"

class QueryParser : public IQueryParser {
 public:
  explicit QueryParser(ISourceExpressionParser *exprParser);
  PQLQueryPtr parseQuery(const PQLQueryString *query) override;

 private:
  QueryLexerFactory lexerFactory;
  ISourceExpressionParser *exprParser;
};
