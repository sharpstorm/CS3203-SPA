#include "SourceLexerTokenTable.h"
#include "common/lexer/AsciiCharacters.h"

SourceLexerTokenTable::SourceLexerTokenTable() {
  for (int i = 0; i < 256; i++) {
    tokens[i] = SIMPLE_TOKEN_INVALID;
  }

  tokens[ASCII_TAB] = SIMPLE_TOKEN_DELIMITER;
  tokens[ASCII_WHITESPACE] = SIMPLE_TOKEN_DELIMITER;
  tokens[ASCII_NEW_LINE] = SIMPLE_TOKEN_DELIMITER;
  tokens[ASCII_CARRIAGE_RETURN] = SIMPLE_TOKEN_IGNORE;

  tokens[ASCII_SEMICOLON] = SIMPLE_TOKEN_SEMICOLON;
  tokens[ASCII_ROUND_BRACKET_LEFT] = SIMPLE_TOKEN_BRACKET_ROUND_LEFT;
  tokens[ASCII_ROUND_BRACKET_RIGHT] = SIMPLE_TOKEN_BRACKET_ROUND_RIGHT;
  tokens[ASCII_CURLY_BRACKET_LEFT] = SIMPLE_TOKEN_BRACKET_CURLY_LEFT;
  tokens[ASCII_CURLY_BRACKET_RIGHT] = SIMPLE_TOKEN_BRACKET_CURLY_RIGHT;

  tokens[ASCII_EXCLAIMATION] = SIMPLE_TOKEN_NOT_PARTIAL;
  tokens[ASCII_PERCENT] = SIMPLE_TOKEN_MOD;
  tokens[ASCII_AMPERSAND] = SIMPLE_TOKEN_AND_PARTIAL;
  tokens[ASCII_PIPE] = SIMPLE_TOKEN_OR_PARTIAL;
  tokens[ASCII_TIMES] = SIMPLE_TOKEN_TIMES;
  tokens[ASCII_PLUS] = SIMPLE_TOKEN_PLUS;
  tokens[ASCII_MINUS] = SIMPLE_TOKEN_MINUS;
  tokens[ASCII_SLASH] = SIMPLE_TOKEN_DIV;

  tokens[ASCII_ANGLE_BRACKET_LEFT] = SIMPLE_TOKEN_LT_PARTIAL;
  tokens[ASCII_ANGLE_BRACKET_RIGHT] = SIMPLE_TOKEN_GT_PARTIAL;
  tokens[ASCII_EQUAL] = SIMPLE_TOKEN_EQUAL_PARTIAL;

  for (int i = ASCII_0; i <= ASCII_9; i++) {
    tokens[i] = SIMPLE_TOKEN_DIGIT;
  }

  for (int i = ASCII_UPPER_A; i <= ASCII_UPPER_Z; i++) {
    tokens[i] = SIMPLE_TOKEN_CHARACTER;
  }

  for (int i = ASCII_LOWER_A; i <= ASCII_LOWER_Z; i++) {
    tokens[i] = SIMPLE_TOKEN_CHARACTER;
  }
}

SourceTokenType SourceLexerTokenTable::lookupToken(const char &c) const {
  return tokens[c];
}

SourceTokenType SourceLexerTokenTable::lookupKeyword(
    const SourceKeywordCandidate &keyword) const {
  const auto &it = keywordMap.find(keyword);
  if (it == keywordMap.end()) {
    return SIMPLE_TOKEN_NULL;
  }
  return it->second;
}

bool SourceLexerTokenTable::isDigit(const char &c) {
  return c >= ASCII_0 && c <= ASCII_9;
}

bool SourceLexerTokenTable::isZero(const char &c) {
  return c == ASCII_0;
}
