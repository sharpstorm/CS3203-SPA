#pragma once

#include <string>
#include "../common/SourceToken.h"
#include "SourceLexerTokenTable.h"
#include "../SPTypes.h"

using std::string;

class SourceLexer {
  typedef string LexerBuffer;
  typedef int LexerCursor;
  typedef int LexerCharacter;
 public:
  SourceLexer();
  SourceTokenStreamPtr tokenize(const FileData &programLines);

 private:
  SourceLexerTokenTable tokenTable;

  SourceTokenType parsePartialSymbol(const SourceTokenType &tokenType,
                                     const LexerBuffer *buffer,
                                     LexerCursor *posPtr);
  template<SourceTokenType twoCharType, SourceTokenType singleCharType>
  SourceTokenType tryGreedySymbolRead(const LexerBuffer *buffer,
                                      LexerCursor *posPtr,
                                      SourceTokenType expectedType);

  template<SourceTokenType twoCharType>
  SourceTokenType twoSymbolAssert(const LexerBuffer *buffer,
                                  LexerCursor *posPtr,
                                  SourceTokenType expectedType);

  bool isNextOfType(const LexerBuffer *buffer,
                    const LexerCursor curPos,
                    SourceTokenType expectedType);
  LexerCharacter peekNextChar(const LexerBuffer *buffer,
                              const LexerCursor curPos);
  SourceToken resolveStringToken(const LexerBuffer &buffer,
                                 const bool hasSeenChar);
  SourceToken validateIdentifier(const LexerBuffer *buffer);
  SourceToken validateIntegerToken(const LexerBuffer *buffer);

  void flushBuffer(SourceTokenStream *result,
                   const LexerBuffer &buffer,
                   const bool hasSeenChar);
};
