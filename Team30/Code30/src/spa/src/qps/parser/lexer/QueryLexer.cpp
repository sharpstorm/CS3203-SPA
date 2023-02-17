#include <stdexcept>
#include <vector>
#include "QueryLexer.h"
#include "qps/errors/QPSLexerError.h"

using std::out_of_range, std::string, std::vector;

const int LEXER_BUFFER_SIZE = 2048;

QueryLexerResult QueryLexer::getTokenStream(string* query) {
  vector<PQLToken>* resultVector = new vector<PQLToken>();

  LexerInternalState lexState{"", false};
  lexState.buffer.reserve(LEXER_BUFFER_SIZE);

  for (size_t pos = 0; pos < query->length(); pos++) {
    char c = query->at(pos);
    processChar(c, resultVector, &lexState);
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
      throw QPSLexerError(QPS_LEXER_ERR_INVALID_CHAR);
    case PQL_TOKEN_IGNORE:
      return;

    case PQL_TOKEN_CHAR:
      state->hasSeenChar = true;
      // Fallthrough
    case PQL_TOKEN_INTEGER:
      state->buffer.push_back(c);
      break;

    default:
      flushBuffer(result, state);
      if (tokenType != PQL_TOKEN_DELIMITER) {
        result->push_back(PQLToken(tokenType));
      }

      state->buffer.clear();
      state->hasSeenChar = false;
  }
}

void QueryLexer::flushBuffer(vector<PQLToken> *result,
                             LexerInternalState* state) {
  if (state->buffer.length() > 0) {
    result->push_back(resolveStringToken(state->buffer, state->hasSeenChar));
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
