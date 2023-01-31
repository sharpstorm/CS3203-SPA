#include "QueryLexer.h"

const char CONST_WHITESPACE = ' ';
const char CONST_TAB = '\t';
const char CONST_NEWLINE = '\n';
const char CONST_RETURN_CARRIAGE = '\r';

vector<string>* QueryLexer::getTokenStream(string* query) {
  string token;

  size_t startPos = 0;
  vector<string>* resultVector = new vector<string>();

  for (size_t pos = 0; pos < query->length(); pos++) {
    char c = query->at(pos);
    switch (c) {
      case CONST_WHITESPACE:
      case CONST_TAB:
      case CONST_NEWLINE:
      case CONST_RETURN_CARRIAGE:
        if (pos - startPos > 0) {
          resultVector->push_back(query->substr(startPos, pos - startPos));
        }
        startPos = pos + 1;
        break;
      default:
        break;
    }
  }

  if (startPos < query->length() - 1) {
    resultVector->push_back(query->substr(startPos,
                                          query->length() - startPos));
  }

  return resultVector;
}
