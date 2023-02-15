#pragma once

#include <string>
#include <vector>
#include "QueryLexerTokenTable.h"
#include "../../../common/UtilityTypes.h"

using std::string, std::vector;
using QueryLexerResult = UniqueVectorPtr<PQLToken>;

class QueryLexer {
 public:
  QueryLexerResult getTokenStream(string *query);

 private:
  QueryLexerTokenTable tokenTable;
  void processChar(char c, vector<PQLToken>* result,
                   bool* hasSeenChar, string* buffer);
  void flushBuffer(vector<PQLToken>* result,
                   bool* hasSeenChar, string* buffer);
  PQLToken resolveStringToken(string buffer, bool hasSeenChar);
  PQLToken validateIntegerToken(string* buffer);
  PQLToken validateIdentifier(string* buffer);
};
