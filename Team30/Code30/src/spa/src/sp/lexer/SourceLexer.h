#pragma once

#include <string>
#include "../common/SourceToken.h"
#include "SourceLexerTokenTable.h"
#include "../SPTypes.h"

using std::string;

class SourceLexer {
  typedef string LexerBuffer;
  typedef int LexerCursor;

 public:
  explicit SourceLexer(const SourceLexerTokenTable *tokenTable,
                       const FileData *programLines);
  SourceTokenStreamPtr tokenize();

 private:
  const SourceLexerTokenTable *tokenTable;
  const FileData *programLines;

  LexerCursor curPos;
  LexerBuffer buffer;
  bool hasSeenChar;
  SourceTokenType deferredPush;

  bool handleToken(const SourceTokenType tokenType, const char c);
  SourceTokenType parsePartialSymbol(const SourceTokenType tokenType);

  template<SourceTokenType twoCharType, SourceTokenType singleCharType>
  SourceTokenType tryGreedySymbolRead(const SourceTokenType expectedType);
  template<SourceTokenType twoCharType>
  SourceTokenType twoSymbolAssert(const SourceTokenType expectedType);

  bool isNextOfType(const SourceTokenType expectedType) const;
  LexerCharacter peekNextChar() const;

  SourceToken resolveStringToken() const;
  SourceToken validateIdentifier() const;
  SourceToken validateIntegerToken() const;

  void flushBufferTo(SourceTokenStream *result) const;
  void resetBuffer();

  static LexerCharacter NO_CHAR;
};
