#include "SourceLexer.h"

#include <memory>

#include "../errors/SPError.h"

using std::make_unique;

const int SOURCE_LEXER_BUFFER_SIZE = 1024;
const int CHAR_MASK = 0xFF;
LexerCharacter SourceLexer::NO_CHAR = -1;

SourceLexer::SourceLexer(const SourceLexerTokenTable *tokenTable,
                         const FileData *programLines) :
    tokenTable(tokenTable), programLines(programLines), curPos(0),
    hasSeenChar(false), deferredPush(SIMPLE_TOKEN_NULL) {
}

SourceTokenStreamPtr SourceLexer::tokenize() {
  SourceTokenStreamPtr resultVector = make_unique<SourceTokenStream>();
  buffer.clear();
  buffer.reserve(SOURCE_LEXER_BUFFER_SIZE);

  for (curPos = 0; curPos < programLines->length(); curPos++) {
    deferredPush = SIMPLE_TOKEN_NULL;

    char c = programLines->at(curPos);
    SourceTokenType tokenType = tokenTable->lookupToken(c);
    if (!handleToken(tokenType, c)) {
      continue;
    }

    flushBufferTo(resultVector.get());
    if (deferredPush != SIMPLE_TOKEN_NULL) {
      resultVector->push_back(SourceToken(deferredPush));
    } else if (!SourceToken::isCategory(tokenType,
                                        SIMPLE_TOKEN_CATEGORY_PROCESSING)) {
      resultVector->push_back(SourceToken(tokenType));
    }

    resetBuffer();
  }

  flushBufferTo(resultVector.get());
  return resultVector;
}

bool SourceLexer::handleToken(const SourceTokenType tokenType,
                              const char c) {
  switch (tokenType) {
    case SIMPLE_TOKEN_INVALID:
      throw SPError(SPERR_INVALID_SOURCE_CHAR);
    case SIMPLE_TOKEN_IGNORE:
      return false;

    case SIMPLE_TOKEN_CHARACTER:
      hasSeenChar = true;
      // Fallthrough
    case SIMPLE_TOKEN_DIGIT:
      buffer.push_back(c);
      return false;

    case SIMPLE_TOKEN_OR_PARTIAL:
    case SIMPLE_TOKEN_AND_PARTIAL:
    case SIMPLE_TOKEN_NOT_PARTIAL:
    case SIMPLE_TOKEN_EQUAL_PARTIAL:
    case SIMPLE_TOKEN_LT_PARTIAL:
    case SIMPLE_TOKEN_GT_PARTIAL:
      deferredPush = parsePartialSymbol(tokenType);
      return true;

    default:
      return true;
  }
}

SourceTokenType SourceLexer::parsePartialSymbol(
    const SourceTokenType tokenType) {
  switch (tokenType) {
    case SIMPLE_TOKEN_OR_PARTIAL:  // || only
      return twoSymbolAssert<SIMPLE_TOKEN_OR>(SIMPLE_TOKEN_OR_PARTIAL);

    case SIMPLE_TOKEN_AND_PARTIAL:  // && only
      return twoSymbolAssert<SIMPLE_TOKEN_AND>(SIMPLE_TOKEN_AND_PARTIAL);

    case SIMPLE_TOKEN_NOT_PARTIAL:  // != or !
      return tryGreedySymbolRead<SIMPLE_TOKEN_NOT_EQUALS, SIMPLE_TOKEN_NOT>(
          SIMPLE_TOKEN_EQUAL_PARTIAL);

    case SIMPLE_TOKEN_EQUAL_PARTIAL:  // == or =
      return tryGreedySymbolRead<SIMPLE_TOKEN_EQUALS, SIMPLE_TOKEN_ASSIGN>(
          SIMPLE_TOKEN_EQUAL_PARTIAL);

    case SIMPLE_TOKEN_LT_PARTIAL:  // <= or <
      return tryGreedySymbolRead<SIMPLE_TOKEN_LTE, SIMPLE_TOKEN_LT>(
          SIMPLE_TOKEN_EQUAL_PARTIAL);

    case SIMPLE_TOKEN_GT_PARTIAL:  // >= or >
      return tryGreedySymbolRead<SIMPLE_TOKEN_GTE, SIMPLE_TOKEN_GT>(
          SIMPLE_TOKEN_EQUAL_PARTIAL);
    default:
      return SIMPLE_TOKEN_NULL;
  }
}

template<SourceTokenType twoCharType, SourceTokenType singleCharType>
SourceTokenType SourceLexer::tryGreedySymbolRead(
    const SourceTokenType expectedType) {
  if (isNextOfType(expectedType)) {
    curPos++;
    return twoCharType;
  } else {
    return singleCharType;
  }
}

template<SourceTokenType twoCharType>
SourceTokenType SourceLexer::twoSymbolAssert(
    const SourceTokenType expectedType) {
  if (!isNextOfType(expectedType)) {
    throw SPError(SPERR_UNKNOWN_TOKEN);
  }

  curPos++;
  return twoCharType;
}

bool SourceLexer::isNextOfType(const SourceTokenType expectedType) const {
  LexerCharacter peek = peekNextChar();
  return ((peek != NO_CHAR)
      && (tokenTable->lookupToken(peek & CHAR_MASK) == expectedType));
}

LexerCharacter SourceLexer::peekNextChar() const {
  if (curPos + 1 < programLines->length()) {
    return programLines->at(curPos + 1);
  }

  return NO_CHAR;
}

void SourceLexer::flushBufferTo(SourceTokenStream *result) const {
  if (buffer.length() > 0) {
    result->push_back(resolveStringToken());
  }
}

SourceToken SourceLexer::resolveStringToken() const {
  SourceTokenType token = tokenTable->lookupKeyword(buffer);
  if (token != SIMPLE_TOKEN_NULL) {
    return SourceToken(token, buffer);
  }

  if (!hasSeenChar) {
    return validateIntegerToken();
  }

  return validateIdentifier();
}

SourceToken SourceLexer::validateIntegerToken() const {
  if (buffer.length() > 1 && SourceLexerTokenTable::isZero(buffer.at(0))) {
    throw SPError(SPERR_INTEGER_STARTS_WITH_ZERO);
  }
  return SourceToken(SIMPLE_TOKEN_INTEGER, buffer);
}

SourceToken SourceLexer::validateIdentifier() const {
  if (SourceLexerTokenTable::isDigit(buffer.at(0))) {
    throw SPError(SPERR_TOKEN_STARTS_WITH_DIGIT);
  }

  return SourceToken(SIMPLE_TOKEN_VARIABLE, buffer);
}

void SourceLexer::resetBuffer() {
  buffer.clear();
  hasSeenChar = false;
}
