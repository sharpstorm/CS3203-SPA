#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../common/SourceToken.h"
#include "SourceLexerTokenTable.h"

using std::string, std::vector, std::unique_ptr;

typedef vector<SourceToken> SourceTokenStream;
typedef unique_ptr<SourceTokenStream> SourceTokenStreamPtr;

class SourceLexer {
 public:
  SourceLexer();
  SourceTokenStreamPtr tokenize(string* programLines);
 private:
  SourceLexerTokenTable tokenTable;

  SourceTokenType parsePartialSymbol(const SourceTokenType &tokenType,
                                     string* buffer,
                                     int *posPtr);
  template<SourceTokenType twoCharType, SourceTokenType singleCharType>
  SourceTokenType tryGreedySymbolRead(string* buffer, int* posPtr, SourceTokenType expectedType);

  template <SourceTokenType twoCharType>
  SourceTokenType twoSymbolAssert(string* buffer, int* posPtr, SourceTokenType expectedType);

  bool isNextOfType(string* buffer, int curPos, SourceTokenType expectedType);
  int peekNextChar(string* buffer, int curPos);
  SourceToken resolveStringToken(string buffer, const bool &hasSeenChar);
  SourceToken validateIdentifier(string *buffer);
  SourceToken validateIntegerToken(string* buffer);

  void flushBuffer(SourceTokenStream* result, string buffer, const bool &hasSeenChar);
};
