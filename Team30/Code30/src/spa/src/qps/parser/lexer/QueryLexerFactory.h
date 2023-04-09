#pragma once

#include "QueryLexer.h"
#include "qps/common/QPSTypes.h"

class QueryLexerFactory {
 public:
  QueryLexerPtr makeLexer(const PQLQueryString *query);

 private:
  QueryLexerTokenTable tokenTable;
};
