#pragma once

#include "QueryLexer.h"

#include <string>

using std::string;

class QueryLexerFactory {
 public:
  QueryLexerPtr makeLexer(string* query);

 private:
  QueryLexerTokenTable tokenTable;
};
