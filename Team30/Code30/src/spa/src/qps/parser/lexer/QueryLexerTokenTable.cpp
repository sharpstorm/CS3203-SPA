#include "QueryLexerTokenTable.h"

const int ASCII_TAB = 9;
const int ASCII_NEW_LINE = 10;
const int ASCII_CARRIAGE_RETURN = 13;
const int ASCII_WHITESPACE = 32;
const int ASCII_QUOTE = 34;
const int ASCII_PERCENT = 37;
const int ASCII_BRACKET_OPEN = 40;
const int ASCII_BRACKET_CLOSE = 41;

const int ASCII_TIMES = 42;
const int ASCII_PLUS = 43;
const int ASCII_COMMA = 44;
const int ASCII_MINUS = 45;
const int ASCII_PERIOD = 46;
const int ASCII_SLASH = 47;

const int ASCII_0 = 48;
const int ASCII_9 = 57;

const int ASCII_SEMICOLON = 59;
const int ASCII_UPPER_A = 65;
const int ASCII_UPPER_Z = 90;

const int ASCII_UNDERSCORE = 95;
const int ASCII_LOWER_A = 97;
const int ASCII_LOWER_Z = 122;

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

  tokens[ASCII_BRACKET_OPEN] = PQL_TOKEN_BRACKET_OPEN;
  tokens[ASCII_BRACKET_CLOSE] = PQL_TOKEN_BRACKET_CLOSE;

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

bool QueryLexerTokenTable::isLetter(char c) {
  return (c >= ASCII_LOWER_A && c <= ASCII_LOWER_Z) ||
      (c >= ASCII_UPPER_Z && c <= ASCII_UPPER_Z);
}

bool QueryLexerTokenTable::isDigit(char c) {
  return c >= ASCII_0 && c <= ASCII_9;
}

bool QueryLexerTokenTable::isZero(char c) {
  return c == ASCII_0;
}
