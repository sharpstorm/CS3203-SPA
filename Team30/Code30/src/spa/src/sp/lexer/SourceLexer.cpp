#include <vector>
#include <stdexcept>
#include "SourceLexer.h"

#include "../errors/SPError.h"

using std::out_of_range;

const int SOURCE_LEXER_BUFFER_SIZE = 1024;

SourceLexer::SourceLexer() = default;

vector<SourceToken> SourceLexer::tokenize(string* programLines) {
  vector<SourceToken> resultVector = vector<SourceToken>();

  bool hasSeenChar = false;
  string buffer;
  buffer.reserve(SOURCE_LEXER_BUFFER_SIZE);

  for (size_t pos = 0; pos < programLines->length(); pos++) {
    SourceTokenType deferredPush = SIMPLE_TOKEN_NULL;

    char c = programLines->at(pos);
    SourceTokenType tokenType = tokenTable.tokens[c];

    switch (tokenType) {
      case SIMPLE_TOKEN_INVALID:
        throw SPError("Invalid Source Character Detected");
      case SIMPLE_TOKEN_IGNORE:
        continue;

      case SIMPLE_TOKEN_NOT_PARTIAL:
        // != or !
        if (isNextOfType(programLines, pos, SIMPLE_TOKEN_EQUAL_PARTIAL)) {
          deferredPush = SIMPLE_TOKEN_NOT_EQUALS;
          pos++;
        } else {
          deferredPush = SIMPLE_TOKEN_NOT;
        }
        break;

      case SIMPLE_TOKEN_OR_PARTIAL:
        // || only
        if (!isNextOfType(programLines, pos, SIMPLE_TOKEN_OR_PARTIAL)) {
          throw SPError("Unknown token |");
        }
        deferredPush = SIMPLE_TOKEN_OR;
        pos++;
        break;

      case SIMPLE_TOKEN_AND_PARTIAL:
        // && only
        if (!isNextOfType(programLines, pos, SIMPLE_TOKEN_AND_PARTIAL)) {
          throw SPError("Unknown token &");
        }
        deferredPush = SIMPLE_TOKEN_AND;
        pos++;
        break;

      case SIMPLE_TOKEN_EQUAL_PARTIAL:
        // =, or ==
        if (isNextOfType(programLines, pos, SIMPLE_TOKEN_EQUAL_PARTIAL)) {
          deferredPush = SIMPLE_TOKEN_EQUALS;
          pos++;
        } else {
          deferredPush = SIMPLE_TOKEN_ASSIGN;
        }
        break;

      case SIMPLE_TOKEN_LT_PARTIAL:
        if (isNextOfType(programLines, pos, SIMPLE_TOKEN_EQUAL_PARTIAL)) {
          deferredPush = SIMPLE_TOKEN_LTE;
          pos++;
        } else {
          deferredPush = SIMPLE_TOKEN_LT;
        }
        break;

      case SIMPLE_TOKEN_GT_PARTIAL:
        if (isNextOfType(programLines, pos, SIMPLE_TOKEN_EQUAL_PARTIAL)) {
          deferredPush = SIMPLE_TOKEN_GTE;
          pos++;
        } else {
          deferredPush = SIMPLE_TOKEN_GT;
        }
        break;

      case SIMPLE_TOKEN_CHARACTER:
        hasSeenChar = true;
        // Fallthrough
      case SIMPLE_TOKEN_DIGIT:
        buffer.push_back(c);
        continue;

      default:
        break;
    }

    if (buffer.length() > 0) {
      resultVector.push_back(resolveStringToken(buffer, hasSeenChar));
    }

    if (deferredPush != SIMPLE_TOKEN_NULL) {
      resultVector.push_back(SourceToken(deferredPush, ""));
    }

    if (!SourceToken::isCategory(tokenType, SIMPLE_TOKEN_CATEGORY_PROCESSING)) {
      resultVector.push_back(SourceToken(tokenType, ""));
    }

    buffer.clear();
    hasSeenChar = false;
  }

  if (buffer.length() > 0) {
    resultVector.push_back(resolveStringToken(buffer, hasSeenChar));
  }

  return resultVector;
}

bool SourceLexer::isNextOfType(string* buffer, int curPos,
                               SourceTokenType expectedType) {
  int peek = peekNextChar(buffer, curPos);
  return (peek >= 0 && tokenTable.tokens[peek] == expectedType);
}

int SourceLexer::peekNextChar(string* buffer, int curPos) {
  if (curPos + 1 < buffer->length()) {
    return buffer->at(curPos + 1);
  }
  return -1;
}

SourceToken SourceLexer::resolveStringToken(string buffer, bool hasSeenChar) {
  try {
    SourceTokenType token = tokenTable.keywordMap.at(buffer);
    return SourceToken(token, buffer);
  } catch (out_of_range&) {
    if (!hasSeenChar) {
      return validateIntegerToken(&buffer);
    }

    return validateIdentifier(&buffer);
  }
}

SourceToken SourceLexer::validateIntegerToken(string* buffer) {
  if (buffer->length() > 1 && tokenTable.isZero(buffer->at(0))) {
    throw SPError("Integer token starts with zero");
  }
  return SourceToken(SIMPLE_TOKEN_INTEGER, *buffer);
}

SourceToken SourceLexer::validateIdentifier(string *buffer) {
  if (tokenTable.isDigit(buffer->at(0))) {
    throw SPError("String token starts with digit");
  }

  return SourceToken(SIMPLE_TOKEN_VARIABLE, *buffer);
}
