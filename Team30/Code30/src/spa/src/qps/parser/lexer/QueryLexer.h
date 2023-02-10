#pragma once

#include <string>
#include <vector>
#include "QueryLexerTokenTable.h"

using std::string, std::vector;

class QueryLexer {
 public:
  vector<PQLToken> getTokenStream(string *query);

 private:
  QueryLexerTokenTable tokenTable;
  PQLToken resolveStringToken(string buffer, bool hasSeenChar);
  PQLToken validateIntegerToken(string* buffer);
  PQLToken validateIdentifier(string* buffer);
};
