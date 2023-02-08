#include "SourceLexerTokenTable.h"

const int ASCII_TAB = 9;
const int ASCII_NEW_LINE = 10;
const int ASCII_CARRIAGE_RETURN = 13;
const int ASCII_WHITESPACE = 32;

const int ASCII_SEMICOLON = 59;
const int ASCII_ROUND_BRACKET_LEFT = 40;
const int ASCII_ROUND_BRACKET_RIGHT = 41;
const int ASCII_CURLY_BRACKET_LEFT = 123;
const int ASCII_CURLY_BRACKET_RIGHT = 125;

const int ASCII_EXCLAIMATION = 33;
const int ASCII_PERCENT = 37;
const int ASCII_TIMES = 42;
const int ASCII_PLUS = 43;
const int ASCII_MINUS = 45;
const int ASCII_SLASH = 47;

const int ASCII_ANGLE_BRACKET_LEFT = 60;
const int ASCII_EQUAL = 61;
const int ASCII_ANGLE_BRACKET_RIGHT = 62;

const int ASCII_AMPERSAND = 38;
const int ASCII_PIPE = 124;

const int ASCII_0 = 48;
const int ASCII_9 = 57;

const int ASCII_UPPER_A = 65;
const int ASCII_UPPER_Z = 90;

const int ASCII_LOWER_A = 97;
const int ASCII_LOWER_Z = 122;

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

bool SourceLexerTokenTable::isLetter(char c) {
  return (c >= ASCII_LOWER_A && c <= ASCII_LOWER_Z) ||
      (c >= ASCII_UPPER_Z && c <= ASCII_UPPER_Z);
}

bool SourceLexerTokenTable::isDigit(char c) {
  return c >= ASCII_0 && c <= ASCII_9;
}

bool SourceLexerTokenTable::isZero(char c) {
  return c == ASCII_0;
}
