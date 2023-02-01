#pragma once

#include <string>
#include <vector>
#include "QueryLexerTokenTable.h"

using std::string, std::vector;

class QueryLexer {
  QueryLexerTokenTable tokenTable;
 public:
  vector<PQLToken> *getTokenStream(string *query);
 private:
  PQLToken resolveStringToken(string buffer, bool hasSeenChar);
};
