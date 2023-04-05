#pragma once

#include <unordered_map>
#include "../common/SourceToken.h"
#include "SPLexerTypes.h"

const char SOURCE_KEYWORD_PROCEDURE[] = "procedure";
const char SOURCE_KEYWORD_READ[] = "read";
const char SOURCE_KEYWORD_PRINT[] = "print";
const char SOURCE_KEYWORD_CALL[] = "call";
const char SOURCE_KEYWORD_WHILE[] = "while";
const char SOURCE_KEYWORD_IF[] = "if";
const char SOURCE_KEYWORD_THEN[] = "then";
const char SOURCE_KEYWORD_ELSE[] = "else";

using std::unordered_map;

class SourceLexerTokenTable {
 private:
  SourceTokenType tokens[256];
  unordered_map<SourceKeyword, SourceTokenType> keywordMap = {
      {SOURCE_KEYWORD_PROCEDURE, SIMPLE_TOKEN_KEYWORD_PROCEDURE},
      {SOURCE_KEYWORD_READ, SIMPLE_TOKEN_KEYWORD_READ},
      {SOURCE_KEYWORD_PRINT, SIMPLE_TOKEN_KEYWORD_PRINT},
      {SOURCE_KEYWORD_CALL, SIMPLE_TOKEN_KEYWORD_CALL},
      {SOURCE_KEYWORD_WHILE, SIMPLE_TOKEN_KEYWORD_WHILE},
      {SOURCE_KEYWORD_IF, SIMPLE_TOKEN_KEYWORD_IF},
      {SOURCE_KEYWORD_THEN, SIMPLE_TOKEN_KEYWORD_THEN},
      {SOURCE_KEYWORD_ELSE, SIMPLE_TOKEN_KEYWORD_ELSE},
  };

 public:
  SourceLexerTokenTable();
  SourceTokenType lookupToken(const LexerCharacter &c) const;
  SourceTokenType lookupKeyword(const SourceKeywordCandidate &keyword) const;
  static bool isDigit(const char &c);
  static bool isZero(const char &c);
};
