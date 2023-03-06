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
      throw QPSLexerError(QPS_LEXER_ERR_INVALID_CHAR);
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

  flushBuffer();
  if (tokenType != PQL_TOKEN_DELIMITER) {
    appendSymbolToken(tokenType, c);
  }
}

void QueryLexer::flushBuffer() {
  if (buffer.length() > 0) {
    appendStringToken(resolveStringToken(buffer, hasSeenChar), buffer);
    clearState();
  }
}

void QueryLexer::toggleLiteral() {
  if (!isProcessingLiteral) {
    startLiteral();
  } else {
    endLiteral();
  }
}

void QueryLexer::startLiteral() {
  literalBuffer.clear();
  isProcessingLiteral = true;
  literalSymbolCount = 0;
}

void QueryLexer::endLiteral() {
  flushBuffer();
  isProcessingLiteral = false;

  if (literalBuffer.empty()) {
    throw QPSLexerError(QPS_LEXER_ERR_EMPTY_QUOTE);
  }

  bool isCharStart = tokenTable->isCharacter(literalBuffer.at(0));
  PQLTokenType type = (literalSymbolCount == 1 && isCharStart) ?
                      PQL_TOKEN_STRING_LITERAL : PQL_TOKEN_LITERAL;
  result->push_back(PQLToken(type, literalBuffer));

  literalBuffer.clear();
  literalSymbolCount = 0;
}

void QueryLexer::appendStringToken(const PQLTokenType &type,
                                   const string &data) {
  if (!isProcessingLiteral) {
    result->push_back(PQLToken(type, data));
    return;
  }

  literalSymbolCount++;
  if (!literalBuffer.empty()) {
    literalBuffer += " ";
  }
  literalBuffer += data;
}

void QueryLexer::appendSymbolToken(const PQLTokenType &type,
                                   const char &c) {
  if (!isProcessingLiteral) {
    result->push_back(PQLToken(type));
    return;
  }

  literalSymbolCount++;
  if (!literalBuffer.empty()) {
    literalBuffer += " ";
  }
  literalBuffer += c;
}

PQLTokenType QueryLexer::resolveStringToken(const string &buffer,
                                            const bool &hasSeenChar) {
  try {
    return tokenTable->keywordMap.at(buffer);
  } catch (out_of_range&) {
    if (!hasSeenChar) {
      validateIntegerToken(buffer);
      return PQL_TOKEN_INTEGER;
    }

    validateIdentifier(buffer);
    return PQL_TOKEN_STRING;
  }
}

void QueryLexer::clearState() {
  buffer.clear();
  hasSeenChar = false;
}

void QueryLexer::validateIntegerToken(const string &buffer) {
  if (buffer.length() > 1 && tokenTable->isZero(buffer.at(0))) {
    throw QPSLexerError(QPS_LEXER_ERR_INTEGER_ZERO);
  }
}

void QueryLexer::validateIdentifier(const string &buffer) {
  if (tokenTable->isDigit(buffer.at(0))) {
    throw QPSLexerError(QPS_LEXER_ERR_STRING_DIGIT);
  }
}
