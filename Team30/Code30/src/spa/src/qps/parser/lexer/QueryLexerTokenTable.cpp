#include "QueryLexerTokenTable.h"
#include "common/lexer/AsciiCharacters.h"

QueryLexerTokenTable::QueryLexerTokenTable(): tokens() {
  for (int i = 0; i < 256; i++) {
    tokens[i] = PQL_TOKEN_INVALID;
  }

  tokens[ASCII_TAB] = PQL_TOKEN_DELIMITER;
  tokens[ASCII_WHITESPACE] = PQL_TOKEN_DELIMITER;
  tokens[ASCII_NEW_LINE] = PQL_TOKEN_DELIMITER;

  tokens[ASCII_CARRIAGE_RETURN] = PQL_TOKEN_IGNORE;

  tokens[ASCII_QUOTE] = PQL_TOKEN_QUOTE;
  tokens[ASCII_NUMBER_SIGN] = PQL_TOKEN_NUMBER_SIGN;
  tokens[ASCII_COMMA] = PQL_TOKEN_COMMA;
  tokens[ASCII_PERIOD] = PQL_TOKEN_PERIOD;
  tokens[ASCII_SEMICOLON] = PQL_TOKEN_SEMICOLON;
  tokens[ASCII_UNDERSCORE] = PQL_TOKEN_UNDERSCORE;
  tokens[ASCII_TIMES] = PQL_TOKEN_ASTRIX;

  tokens[ASCII_PLUS] = PQL_TOKEN_VALID;
  tokens[ASCII_MINUS] = PQL_TOKEN_VALID;
  tokens[ASCII_SLASH] = PQL_TOKEN_VALID;
  tokens[ASCII_PERCENT] = PQL_TOKEN_VALID;

  tokens[ASCII_ROUND_BRACKET_LEFT] = PQL_TOKEN_BRACKET_OPEN;
  tokens[ASCII_ROUND_BRACKET_RIGHT] = PQL_TOKEN_BRACKET_CLOSE;
  tokens[ASCII_ANGLE_BRACKET_LEFT] = PQL_TOKEN_TUPLE_OPEN;
  tokens[ASCII_EQUAL] = PQL_TOKEN_EQUALS;
  tokens[ASCII_ANGLE_BRACKET_RIGHT] = PQL_TOKEN_TUPLE_CLOSE;

  for (int i = ASCII_0; i <= ASCII_9; i++) {
    tokens[i] = PQL_TOKEN_INTEGER;
  }

  for (int i = ASCII_UPPER_A; i <= ASCII_UPPER_Z; i++) {
    tokens[i] = PQL_TOKEN_CHAR;
  }

  for (int i = ASCII_LOWER_A; i <= ASCII_LOWER_Z; i++) {
    tokens[i] = PQL_TOKEN_CHAR;
  }
}

PQLTokenType QueryLexerTokenTable::lookupToken(const QueryCharacter &c) const {
  return tokens[c];
}

PQLTokenType QueryLexerTokenTable::lookupKeyword(
    const QueryKeywordCandidate &keyword) const {
  const auto &it = keywordMap.find(keyword);
  if (it == keywordMap.end()) {
    return PQL_TOKEN_NULL;
  }
  return it->second;
}

bool QueryLexerTokenTable::isDigit(const char c) {
  return c >= ASCII_0 && c <= ASCII_9;
}

bool QueryLexerTokenTable::isCharacter(const char c) {
  return (c >= ASCII_UPPER_A && c <= ASCII_UPPER_Z)
      || (c >= ASCII_LOWER_A && c <= ASCII_LOWER_Z);
}

bool QueryLexerTokenTable::isZero(const char c) {
  return c == ASCII_0;
}
