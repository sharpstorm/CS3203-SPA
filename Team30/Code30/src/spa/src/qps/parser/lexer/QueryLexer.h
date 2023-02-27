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
  struct LexerInternalState {
    string buffer;
    bool hasSeenChar;
    bool isLiteralVarchar;
    bool isProcessingLiteral;
  };

  QueryLexerTokenTable tokenTable;
  void processChar(char c, vector<PQLToken>* result,
                   LexerInternalState* state);
  void flushBuffer(vector<PQLToken>* result,
                   LexerInternalState* state);
  void toggleLiteral(vector<PQLToken>* result,
                     LexerInternalState* state);
  void startLiteral(LexerInternalState* state);
  void flushLiteral(vector<PQLToken>* result,
                    LexerInternalState* state);
  void clearState(LexerInternalState* state);
  PQLToken resolveStringToken(string buffer, bool hasSeenChar);
  PQLToken validateIntegerToken(string* buffer);
  PQLToken validateIdentifier(string* buffer);
  void throwInvalidCharError(LexerInternalState* state);
};
