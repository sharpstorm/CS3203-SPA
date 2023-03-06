#include <stdexcept>
#include <vector>
#include <utility>

#include "QueryLexer.h"
#include "qps/errors/QPSLexerError.h"

using std::out_of_range, std::string, std::vector;

const int LEXER_BUFFER_SIZE = 2048;

QueryLexer::QueryLexer(string *query, QueryLexerTokenTable* tokenTable):
    query(query),
    tokenTable(tokenTable),
    result(make_unique<PQLTokenStream>()),
    buffer(""),
    literalBuffer(""),
    isProcessingLiteral(false),
    hasSeenChar(false),
    literalSymbolCount(0) {
  buffer.reserve(LEXER_BUFFER_SIZE);
  literalBuffer.reserve(LEXER_BUFFER_SIZE);
}

PQLTokenStreamPtr QueryLexer::getTokenStream() {
  for (size_t pos = 0; pos < query->length(); pos++) {
    char c = query->at(pos);
    processChar(c);
  }

  if (isProcessingLiteral) {
    throw QPSLexerError(QPS_LEXER_ERR_UNMATCHED_QUOTE);
  }

  flushBuffer();
  return std::move(result);
}

void QueryLexer::processChar(const char &c) {
  PQLTokenType tokenType = tokenTable->tokens[c];

  switch (tokenType) {
    case PQL_TOKEN_INVALID:
      throwInvalidCharError();
      break;
    case PQL_TOKEN_IGNORE:
      return;

    case PQL_TOKEN_CHAR:
      hasSeenChar = true;
      // Fallthrough
    case PQL_TOKEN_INTEGER:
      buffer.push_back(c);
      return;

    case PQL_TOKEN_QUOTE:
      toggleLiteral();
      return;
    default:
      break;
  }

  if (isProcessingLiteral) {
    processLiteral(c, tokenType);
    return;
  }

  flushBuffer();
  if (tokenType != PQL_TOKEN_DELIMITER) {
    result->push_back(PQLToken(tokenType));
  }
}

void QueryLexer::processLiteral(const char &c, const PQLTokenType &type) {
  if (type != PQL_TOKEN_DELIMITER) {
    if (buffer.empty()) {
      literalSymbolCount++;
    } else {
      buffer.push_back(' ');
    }
    buffer.push_back(c);
  }

  if (buffer.empty()) {
    return;
  }

  literalSymbolCount++;
  if (!literalBuffer.empty()) {
    literalBuffer += " ";
  }
  literalBuffer += buffer;
  buffer.clear();
  return;
}

void QueryLexer::flushBuffer() {
  if (buffer.length() > 0) {
    result->push_back(resolveStringToken(buffer, hasSeenChar));
    clearState();
  }
}

void QueryLexer::toggleLiteral() {
  if (isProcessingLiteral) {
    flushLiteral();
  } else {
    startLiteral();
  }
}

void QueryLexer::startLiteral() {
  isProcessingLiteral = true;
  literalSymbolCount = 0;
}

void QueryLexer::flushLiteral() {
  if (!buffer.empty()) {
    literalSymbolCount++;
    if (!literalBuffer.empty()) {
      literalBuffer += " ";
    }
    literalBuffer += buffer;
  }

  if (literalBuffer.length() > 0) {
    PQLTokenType type = (literalSymbolCount == 1) ?
                        PQL_TOKEN_STRING_LITERAL : PQL_TOKEN_LITERAL;
    if (tokenTable->isDigit(literalBuffer.at(0))) {
      type = PQL_TOKEN_LITERAL;
    }
    result->push_back(PQLToken(type, literalBuffer));
    clearState();
  } else {
    throw QPSLexerError(QPS_LEXER_ERR_EMPTY_QUOTE);
  }
}

PQLToken QueryLexer::resolveStringToken(string buffer, bool hasSeenChar) {
  try {
    PQLTokenType token = tokenTable->keywordMap.at(buffer);
    return PQLToken(token, buffer);
  } catch (out_of_range&) {
    if (!hasSeenChar) {
      return validateIntegerToken(&buffer);
    }

    return validateIdentifier(&buffer);
  }
}

void QueryLexer::clearState() {
  buffer.clear();
  literalBuffer.clear();
  hasSeenChar = false;
  isProcessingLiteral = false;
  literalSymbolCount = 0;
}

PQLToken QueryLexer::validateIntegerToken(string* buffer) {
  if (buffer->length() > 1 && tokenTable->isZero(buffer->at(0))) {
    throw QPSLexerError(QPS_LEXER_ERR_INTEGER_ZERO);
  }
  return PQLToken(PQL_TOKEN_INTEGER, *buffer);
}

PQLToken QueryLexer::validateIdentifier(string *buffer) {
  if (tokenTable->isDigit(buffer->at(0))) {
    throw QPSLexerError(QPS_LEXER_ERR_STRING_DIGIT);
  }

  return PQLToken(PQL_TOKEN_STRING, *buffer);
}

void QueryLexer::throwInvalidCharError() {
  if (!isProcessingLiteral) {
    throw QPSLexerError(QPS_LEXER_ERR_INVALID_CHAR);
  }
}
