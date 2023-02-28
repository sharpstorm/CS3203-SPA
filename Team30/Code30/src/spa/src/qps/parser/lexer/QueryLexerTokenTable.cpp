#include "QueryLexerTokenTable.h"
#include "common/lexer/AsciiCharacters.h"

QueryLexerTokenTable::QueryLexerTokenTable() {
  for (int i = 0; i < 256; i++) {
    tokens[i] = PQL_TOKEN_INVALID;
  }

  tokens[ASCII_TAB] = PQL_TOKEN_DELIMITER;
  tokens[ASCII_WHITESPACE] = PQL_TOKEN_DELIMITER;
  tokens[ASCII_NEW_LINE] = PQL_TOKEN_DELIMITER;

  tokens[ASCII_CARRIAGE_RETURN] = PQL_TOKEN_IGNORE;

  tokens[ASCII_QUOTE] = PQL_TOKEN_QUOTE;
  tokens[ASCII_COMMA] = PQL_TOKEN_COMMA;
  tokens[ASCII_PERIOD] = PQL_TOKEN_PERIOD;
  tokens[ASCII_SEMICOLON] = PQL_TOKEN_SEMICOLON;
  tokens[ASCII_UNDERSCORE] = PQL_TOKEN_UNDERSCORE;
  tokens[ASCII_TIMES] = PQL_TOKEN_ASTRIX;

  tokens[ASCII_ROUND_BRACKET_LEFT] = PQL_TOKEN_BRACKET_OPEN;
  tokens[ASCII_ROUND_BRACKET_RIGHT] = PQL_TOKEN_BRACKET_CLOSE;

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

bool QueryLexerTokenTable::isDigit(char c) {
  return c >= ASCII_0 && c <= ASCII_9;
}

bool QueryLexerTokenTable::isZero(char c) {
  return c == ASCII_0;
}
