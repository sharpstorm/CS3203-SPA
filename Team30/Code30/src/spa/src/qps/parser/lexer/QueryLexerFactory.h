#pragma once

#include "QueryLexer.h"

#include <string>

using std::string;

class QueryLexerFactory {
 public:
  QueryLexerPtr makeLexer(const string *query);

 private:
  QueryLexerTokenTable tokenTable;
};
