#include <stdexcept>
#include "QueryLexer.h"
#include "qps/errors/QPSLexerError.h"

using std::out_of_range, std::string;

const int LEXER_BUFFER_SIZE = 2048;

vector<PQLToken>* QueryLexer::getTokenStream(string* query) {
  vector<PQLToken>* resultVector = new vector<PQLToken>();

  bool hasSeenChar = false;
  string buffer;
  buffer.reserve(LEXER_BUFFER_SIZE);

  for (size_t pos = 0; pos < query->length(); pos++) {
    char c = query->at(pos);
    PQL_TOKEN_TYPE tokenType = tokenTable.tokens[c];

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
          resultVector->push_back(resolveStringToken(buffer, hasSeenChar));
        }

        if (tokenType != PQL_TOKEN_DELIMITER) {
          resultVector->push_back(PQLToken{
            tokenType
          });
        }

        buffer.clear();
        hasSeenChar = false;
    }
  }

  if (buffer.length() > 0) {
    resultVector->push_back(resolveStringToken(buffer, hasSeenChar));
  }

  return resultVector;
}

PQLToken QueryLexer::resolveStringToken(string buffer, bool hasSeenChar) {
  try {
    PQL_TOKEN_TYPE token = tokenTable.keywordMap.at(buffer);
    return PQLToken{
        token,
        buffer
    };
  } catch (out_of_range&) {
    string tokenCopy = buffer;
    return PQLToken{
        hasSeenChar ? PQL_TOKEN_STRING : PQL_TOKEN_INTEGER,
        buffer
    };
  }
}
