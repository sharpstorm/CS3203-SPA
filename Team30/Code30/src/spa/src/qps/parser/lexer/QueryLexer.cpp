#include <stdexcept>
#include <vector>
#include "QueryLexer.h"
#include "qps/errors/QPSLexerError.h"

using std::out_of_range, std::string, std::vector;

const int LEXER_BUFFER_SIZE = 2048;

QueryLexerResult QueryLexer::getTokenStream(string* query) {
  vector<PQLToken>* resultVector = new vector<PQLToken>();

  LexerInternalState lexState{"", false, false, false};
  lexState.buffer.reserve(LEXER_BUFFER_SIZE);

  for (size_t pos = 0; pos < query->length(); pos++) {
    char c = query->at(pos);
    processChar(c, resultVector, &lexState);
  }

  if (lexState.isProcessingLiteral) {
    throw QPSLexerError(QPS_LEXER_ERR_UNMATCHED_QUOTE);
  }
  flushBuffer(resultVector, &lexState);
  return QueryLexerResult(resultVector);
}

void QueryLexer::processChar(char c,
                             vector<PQLToken> *result,
                             LexerInternalState* state) {
  PQLTokenType tokenType = tokenTable.tokens[c];

  switch (tokenType) {
    case PQL_TOKEN_INVALID:
      throwInvalidCharError(state);
      break;
    case PQL_TOKEN_IGNORE:
      return;

    case PQL_TOKEN_CHAR:
      state->hasSeenChar = true;
      // Fallthrough
    case PQL_TOKEN_INTEGER:
      state->buffer.push_back(c);
      return;

    case PQL_TOKEN_QUOTE:
      toggleLiteral(result, state);
      return;
    default:
      break;
  }

  if (state->isProcessingLiteral) {
    state->isLiteralVarchar = false;
    state->buffer.push_back(c);
    return;
  }

  flushBuffer(result, state);
  if (tokenType != PQL_TOKEN_DELIMITER) {
    result->push_back(PQLToken(tokenType));
  }
}

void QueryLexer::flushBuffer(vector<PQLToken> *result,
                             LexerInternalState* state) {
  if (state->buffer.length() > 0) {
    result->push_back(resolveStringToken(state->buffer, state->hasSeenChar));
    clearState(state);
  }
}

void QueryLexer::toggleLiteral(vector<PQLToken> *result,
                               LexerInternalState *state) {
  if (state->isProcessingLiteral) {
    flushLiteral(result, state);
  } else {
    startLiteral(state);
  }
}

void QueryLexer::startLiteral(LexerInternalState *state) {
  state->isProcessingLiteral = true;
  state->isLiteralVarchar = true;
}

void QueryLexer::flushLiteral(vector<PQLToken> *result,
                              LexerInternalState* state) {
  if (state->buffer.length() > 0) {
    PQLTokenType type = (state->isLiteralVarchar) ?
                        PQL_TOKEN_STRING_LITERAL : PQL_TOKEN_LITERAL;
    if (tokenTable.isDigit(state->buffer.at(0))) {
      type = PQL_TOKEN_LITERAL;
    }
    result->push_back(PQLToken(type, state->buffer));
    clearState(state);
  } else {
    throw QPSLexerError(QPS_LEXER_ERR_EMPTY_QUOTE);
  }
}

PQLToken QueryLexer::resolveStringToken(string buffer, bool hasSeenChar) {
  try {
    PQLTokenType token = tokenTable.keywordMap.at(buffer);
    return PQLToken(token, buffer);
  } catch (out_of_range&) {
    if (!hasSeenChar) {
      return validateIntegerToken(&buffer);
    }

    return validateIdentifier(&buffer);
  }
}

void QueryLexer::clearState(LexerInternalState *state) {
  state->buffer.clear();
  state->hasSeenChar = false;
  state->isProcessingLiteral = false;
  state->isLiteralVarchar = true;
}

PQLToken QueryLexer::validateIntegerToken(string* buffer) {
  if (buffer->length() > 1 && tokenTable.isZero(buffer->at(0))) {
    throw QPSLexerError(QPS_LEXER_ERR_INTEGER_ZERO);
  }
  return PQLToken(PQL_TOKEN_INTEGER, *buffer);
}

PQLToken QueryLexer::validateIdentifier(string *buffer) {
  if (tokenTable.isDigit(buffer->at(0))) {
    throw QPSLexerError(QPS_LEXER_ERR_STRING_DIGIT);
  }

  return PQLToken(PQL_TOKEN_STRING, *buffer);
}

void QueryLexer::throwInvalidCharError(LexerInternalState* state) {
  if (!state->isProcessingLiteral) {
    throw QPSLexerError(QPS_LEXER_ERR_INVALID_CHAR);
  }
}
