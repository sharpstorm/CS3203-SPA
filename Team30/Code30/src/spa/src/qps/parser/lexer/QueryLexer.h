#pragma once

#include <string>
#include "QueryLexerTokenTable.h"
#include "../../../common/UtilityTypes.h"

using std::string, std::vector;
using QueryLexerResult = UniqueVectorPtr<PQLToken>;

class QueryLexer {
 public:
  QueryLexerResult getTokenStream(string *query);

 private:
  QueryLexerTokenTable tokenTable;
  PQLToken resolveStringToken(string buffer, bool hasSeenChar);
  PQLToken validateIntegerToken(string* buffer);
  PQLToken validateIdentifier(string* buffer);
};
