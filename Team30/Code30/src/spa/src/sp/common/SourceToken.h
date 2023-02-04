#pragma once

#include <string>

using std::string;

enum SourceTokenCategory {
  SIMPLE_TOKEN_CATEGORY_SYNTATIC = 0x100,
  SIMPLE_TOKEN_CATEGORY_ARITHMETIC = 0x200,
  SIMPLE_TOKEN_CATEGORY_KEYWORD = 0x400,
  SIMPLE_TOKEN_CATEGORY_PROCESSING = 0x800,
  SIMPLE_TOKEN_CATEGORY_INTEGER = 0x1000,
  SIMPLE_TOKEN_CATEGORY_VARCHAR = 0x2000,
};

enum SourceTokenType {
  SIMPLE_TOKEN_NULL = 0,

  SIMPLE_TOKEN_BRACKET_ROUND_LEFT = SIMPLE_TOKEN_CATEGORY_SYNTATIC,
  SIMPLE_TOKEN_BRACKET_ROUND_RIGHT,
  SIMPLE_TOKEN_BRACKET_CURLY_LEFT,
  SIMPLE_TOKEN_BRACKET_CURLY_RIGHT,
  SIMPLE_TOKEN_SEMICOLON,
  SIMPLE_TOKEN_ASSIGN,

  SIMPLE_TOKEN_NOT = SIMPLE_TOKEN_CATEGORY_ARITHMETIC,
  SIMPLE_TOKEN_AND,
  SIMPLE_TOKEN_OR,
  SIMPLE_TOKEN_GT,
  SIMPLE_TOKEN_GTE,
  SIMPLE_TOKEN_LT,
  SIMPLE_TOKEN_LTE,
  SIMPLE_TOKEN_EQUALS,
  SIMPLE_TOKEN_NOT_EQUALS,
  SIMPLE_TOKEN_PLUS,
  SIMPLE_TOKEN_MINUS,
  SIMPLE_TOKEN_TIMES,
  SIMPLE_TOKEN_DIV,
  SIMPLE_TOKEN_MOD,

  SIMPLE_TOKEN_KEYWORD_PROCEDURE = SIMPLE_TOKEN_CATEGORY_KEYWORD,
  SIMPLE_TOKEN_KEYWORD_READ,
  SIMPLE_TOKEN_KEYWORD_PRINT,
  SIMPLE_TOKEN_KEYWORD_CALL,
  SIMPLE_TOKEN_KEYWORD_WHILE,
  SIMPLE_TOKEN_KEYWORD_IF,
  SIMPLE_TOKEN_KEYWORD_THEN,
  SIMPLE_TOKEN_KEYWORD_ELSE,

  SIMPLE_TOKEN_DIGIT = SIMPLE_TOKEN_CATEGORY_PROCESSING,
  SIMPLE_TOKEN_CHARACTER,
  SIMPLE_TOKEN_DELIMITER,
  SIMPLE_TOKEN_IGNORE,
  SIMPLE_TOKEN_INVALID,
  SIMPLE_TOKEN_AND_PARTIAL,
  SIMPLE_TOKEN_OR_PARTIAL,
  SIMPLE_TOKEN_EQUAL_PARTIAL,
  SIMPLE_TOKEN_NOT_PARTIAL,
  SIMPLE_TOKEN_LT_PARTIAL,
  SIMPLE_TOKEN_GT_PARTIAL,

  SIMPLE_TOKEN_VARIABLE = SIMPLE_TOKEN_CATEGORY_VARCHAR,
  SIMPLE_TOKEN_INTEGER = SIMPLE_TOKEN_CATEGORY_INTEGER,
};

const int SIMPLE_TOKEN_CATEGORY_MASK = 0xFFFFFF00;
const int SIMPLE_VARCHAR_MASK = SIMPLE_TOKEN_CATEGORY_VARCHAR |
    SIMPLE_TOKEN_CATEGORY_KEYWORD;

class SourceToken {
 private:
  SourceTokenType type;
  string value;

 public:
  SourceToken(SourceTokenType type, string value);
  SourceTokenType getType();
  string getValue();
  bool isType(SourceTokenType target);
  bool isCategory(SourceTokenCategory target);
  static bool isCategory(SourceTokenType type, SourceTokenCategory target);
  bool operator==(const SourceToken& other) const;
};
