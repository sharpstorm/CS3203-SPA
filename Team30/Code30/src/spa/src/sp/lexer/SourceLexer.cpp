#include "SourceLexer.h"

#include <memory>

#include "../errors/SPError.h"

using std::make_unique;

const int SOURCE_LEXER_BUFFER_SIZE = 1024;

SourceLexer::SourceLexer() = default;

SourceTokenStreamPtr SourceLexer::tokenize(const FileData &programLines) {
  SourceTokenStreamPtr resultVector = make_unique<SourceTokenStream>();

  bool hasSeenChar = false;
  LexerBuffer buffer;
  buffer.reserve(SOURCE_LEXER_BUFFER_SIZE);

  for (LexerCursor pos = 0; pos < programLines.length(); pos++) {
    SourceTokenType deferredPush = SIMPLE_TOKEN_NULL;
    char c = programLines.at(pos);
    SourceTokenType tokenType = tokenTable.lookupToken(c);

    switch (tokenType) {
      case SIMPLE_TOKEN_INVALID:
        throw SPError(SPERR_INVALID_SOURCE_CHAR);
      case SIMPLE_TOKEN_IGNORE:
        continue;

      case SIMPLE_TOKEN_OR_PARTIAL:
      case SIMPLE_TOKEN_AND_PARTIAL:
      case SIMPLE_TOKEN_NOT_PARTIAL:
      case SIMPLE_TOKEN_EQUAL_PARTIAL:
      case SIMPLE_TOKEN_LT_PARTIAL:
      case SIMPLE_TOKEN_GT_PARTIAL:
        deferredPush = parsePartialSymbol(tokenType, &programLines, &pos);
        break;

      case SIMPLE_TOKEN_CHARACTER:
        hasSeenChar = true;
        // Fallthrough
      case SIMPLE_TOKEN_DIGIT:
        buffer.push_back(c);
        continue;

      default:break;
    }

    flushBuffer(resultVector.get(), buffer, hasSeenChar);
    if (deferredPush != SIMPLE_TOKEN_NULL) {
      resultVector->push_back(SourceToken(deferredPush, ""));
    } else if (!SourceToken::isCategory(tokenType,
                                        SIMPLE_TOKEN_CATEGORY_PROCESSING)) {
      resultVector->push_back(SourceToken(tokenType, ""));
    }

    buffer.clear();
    hasSeenChar = false;
  }

  flushBuffer(resultVector.get(), buffer, hasSeenChar);
  return resultVector;
}

void SourceLexer::flushBuffer(SourceTokenStream *result,
                              const LexerBuffer &buffer,
                              const bool hasSeenChar) {
  if (buffer.length() > 0) {
    result->push_back(resolveStringToken(buffer, hasSeenChar));
  }
}

SourceTokenType SourceLexer::parsePartialSymbol(
    const SourceTokenType &tokenType,
    const string *buffer,
    LexerCursor *posPtr) {
  switch (tokenType) {
    case SIMPLE_TOKEN_OR_PARTIAL:
      // || only
      return twoSymbolAssert<SIMPLE_TOKEN_OR>(
          buffer, posPtr, SIMPLE_TOKEN_OR_PARTIAL);

    case SIMPLE_TOKEN_AND_PARTIAL:
      // && only
      return twoSymbolAssert<SIMPLE_TOKEN_AND>(
          buffer, posPtr, SIMPLE_TOKEN_AND_PARTIAL);

    case SIMPLE_TOKEN_NOT_PARTIAL:
      // != or !
      return tryGreedySymbolRead<SIMPLE_TOKEN_NOT_EQUALS, SIMPLE_TOKEN_NOT>(
          buffer, posPtr, SIMPLE_TOKEN_EQUAL_PARTIAL);

    case SIMPLE_TOKEN_EQUAL_PARTIAL:
      // == or =
      return tryGreedySymbolRead<SIMPLE_TOKEN_EQUALS, SIMPLE_TOKEN_ASSIGN>(
          buffer, posPtr, SIMPLE_TOKEN_EQUAL_PARTIAL);

    case SIMPLE_TOKEN_LT_PARTIAL:
      // <= or <
      return tryGreedySymbolRead<SIMPLE_TOKEN_LTE, SIMPLE_TOKEN_LT>(
          buffer, posPtr, SIMPLE_TOKEN_EQUAL_PARTIAL);

    case SIMPLE_TOKEN_GT_PARTIAL:
      // >= or >
      return tryGreedySymbolRead<SIMPLE_TOKEN_GTE, SIMPLE_TOKEN_GT>(
          buffer, posPtr, SIMPLE_TOKEN_EQUAL_PARTIAL);
    default:return SIMPLE_TOKEN_NULL;
  }
}

template<SourceTokenType twoCharType, SourceTokenType singleCharType>
SourceTokenType SourceLexer::tryGreedySymbolRead(const LexerBuffer *buffer,
                                                 LexerCursor *posPtr,
                                                 SourceTokenType expectedType) {
  if (isNextOfType(buffer, *posPtr, expectedType)) {
    (*posPtr)++;
    return twoCharType;
  } else {
    return singleCharType;
  }
}

template<SourceTokenType twoCharType>
SourceTokenType SourceLexer::twoSymbolAssert(const LexerBuffer *buffer,
                                             LexerCursor *posPtr,
                                             SourceTokenType expectedType) {
  if (!isNextOfType(buffer, *posPtr, expectedType)) {
    throw SPError(SPERR_UNKNOWN_TOKEN);
  }
  (*posPtr)++;
  return twoCharType;
}

bool SourceLexer::isNextOfType(const LexerBuffer *buffer,
                               const LexerCursor curPos,
                               SourceTokenType expectedType) {
  LexerCharacter peek = peekNextChar(buffer, curPos);
  return (peek >= 0 && tokenTable.lookupToken(peek) == expectedType);
}

SourceLexer::LexerCharacter SourceLexer::peekNextChar(
    const LexerBuffer *buffer, const LexerCursor curPos) {
  if (curPos + 1 < buffer->length()) {
    return buffer->at(curPos + 1);
  }
  return -1;
}

SourceToken SourceLexer::resolveStringToken(const LexerBuffer &buffer,
                                            const bool hasSeenChar) {
  SourceTokenType token = tokenTable.lookupKeyword(buffer);
  if (token != SIMPLE_TOKEN_NULL) {
    return SourceToken(token, buffer);
  }

  if (!hasSeenChar) {
    return validateIntegerToken(&buffer);
  }

  return validateIdentifier(&buffer);
}

SourceToken SourceLexer::validateIntegerToken(const LexerBuffer *buffer) {
  if (buffer->length() > 1 && tokenTable.isZero(buffer->at(0))) {
    throw SPError(SPERR_INTEGER_STARTS_WITH_ZERO);
  }
  return SourceToken(SIMPLE_TOKEN_INTEGER, *buffer);
}

SourceToken SourceLexer::validateIdentifier(const LexerBuffer *buffer) {
  if (tokenTable.isDigit(buffer->at(0))) {
    throw SPError(SPERR_TOKEN_STARTS_WITH_DIGIT);
  }

  return SourceToken(SIMPLE_TOKEN_VARIABLE, *buffer);
}
