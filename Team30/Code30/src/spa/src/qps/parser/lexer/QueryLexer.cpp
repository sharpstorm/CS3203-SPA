#include <stdexcept>
#include "QueryLexer.h"
#include "qps/errors/QPSLexerError.h"

using std::out_of_range, std::string;

const int LEXER_BUFFER_SIZE = 2048;

vector<PQLToken> QueryLexer::getTokenStream(string* query) {
  vector<PQLToken> resultVector = vector<PQLToken>();

  bool hasSeenChar = false;
  string buffer;
  buffer.reserve(LEXER_BUFFER_SIZE);

  for (size_t pos = 0; pos < query->length(); pos++) {
    char c = query->at(pos);
    PQLTokenType tokenType = tokenTable.tokens[c];

    switch (tokenType) {
      case PQL_TOKEN_INVALID:
        throw QPSLexerError("Detected invalid token");
      case PQL_TOKEN_IGNORE:
        continue;

      case PQL_TOKEN_CHAR:
        hasSeenChar = true;
        // Fallthrough
      case PQL_TOKEN_INTEGER:
        buffer.push_back(c);
        break;

      default:
        if (buffer.length() > 0) {
          resultVector.push_back(resolveStringToken(buffer, hasSeenChar));
        }

        if (tokenType != PQL_TOKEN_DELIMITER) {
          resultVector.push_back(PQLToken{
            tokenType
          });
        }

        buffer.clear();
        hasSeenChar = false;
    }
  }

  if (buffer.length() > 0) {
    resultVector.push_back(resolveStringToken(buffer, hasSeenChar));
  }

  return resultVector;
}

PQLToken QueryLexer::resolveStringToken(string buffer, bool hasSeenChar) {
  try {
    PQLTokenType token = tokenTable.keywordMap.at(buffer);
    return PQLToken{
        token,
        buffer
    };
  } catch (out_of_range&) {
    if (!hasSeenChar) {
      return validateIntegerToken(&buffer);
    }

    return validateIdentifier(&buffer);
  }
}

PQLToken QueryLexer::validateIntegerToken(string* buffer) {
  if (buffer->length() > 1 && tokenTable.isZero(buffer->at(0))) {
    throw QPSLexerError("Integer token starts with zero");
  }
  return PQLToken{
      PQL_TOKEN_INTEGER,
      *buffer
  };
}

PQLToken QueryLexer::validateIdentifier(string *buffer) {
  if (tokenTable.isDigit(buffer->at(0))) {
    throw QPSLexerError("String token starts with digit");
  }

  return PQLToken{
    PQL_TOKEN_STRING,
    *buffer
  };
}