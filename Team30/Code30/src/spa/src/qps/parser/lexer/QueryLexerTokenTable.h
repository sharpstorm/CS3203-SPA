#pragma once

#include <unordered_map>
#include <string>

#include "../PQLToken.h"
#include "qps/parser/common/QueryKeywords.h"

using std::string, std::unordered_map;

typedef string QueryKeyword;
typedef QueryKeyword QueryKeywordCandidate;
typedef int QueryCharacter;

class QueryLexerTokenTable {
 protected:
  unordered_map<QueryKeyword, PQLTokenType> keywordMap = {
      {PQL_KEYWORD_SELECT, PQL_TOKEN_SELECT},
      {PQL_KEYWORD_STMT, PQL_TOKEN_STMT},
      {PQL_KEYWORD_READ, PQL_TOKEN_READ},
      {PQL_KEYWORD_PRINT, PQL_TOKEN_PRINT},
      {PQL_KEYWORD_CALL, PQL_TOKEN_CALL},
      {PQL_KEYWORD_WHILE, PQL_TOKEN_WHILE},
      {PQL_KEYWORD_IF, PQL_TOKEN_IF},
      {PQL_KEYWORD_ASSIGN, PQL_TOKEN_ASSIGN},
      {PQL_KEYWORD_VARIABLE, PQL_TOKEN_VARIABLE},
      {PQL_KEYWORD_CONSTANT, PQL_TOKEN_CONSTANT},
      {PQL_KEYWORD_PROCEDURE, PQL_TOKEN_PROCEDURE},
      {PQL_KEYWORD_FOLLOWS, PQL_TOKEN_FOLLOWS},
      {PQL_KEYWORD_PARENT, PQL_TOKEN_PARENT},
      {PQL_KEYWORD_USES, PQL_TOKEN_USES},
      {PQL_KEYWORD_MODIFIES, PQL_TOKEN_MODIFIES},
      {PQL_KEYWORD_PATTERN, PQL_TOKEN_PATTERN},
      {PQL_KEYWORD_SUCH, PQL_TOKEN_SUCH},
      {PQL_KEYWORD_THAT, PQL_TOKEN_THAT},
      {PQL_KEYWORD_CALLS, PQL_TOKEN_CALLS},
      {PQL_KEYWORD_AND, PQL_TOKEN_AND},
      {PQL_KEYWORD_WITH, PQL_TOKEN_WITH},
      {PQL_KEYWORD_PROC_NAME, PQL_TOKEN_PROC_NAME},
      {PQL_KEYWORD_VAR_NAME, PQL_TOKEN_VAR_NAME},
      {PQL_KEYWORD_VALUE, PQL_TOKEN_VALUE},
      {PQL_KEYWORD_NEXT, PQL_TOKEN_NEXT},
      {PQL_KEYWORD_AFFECTS, PQL_TOKEN_AFFECTS},
      {PQL_KEYWORD_BOOLEAN, PQL_TOKEN_BOOLEAN}
  };
  PQLTokenType tokens[256];

 public:
  QueryLexerTokenTable();
  PQLTokenType lookupToken(const QueryCharacter &c) const;
  PQLTokenType lookupKeyword(const QueryKeywordCandidate &keyword) const;

  static bool isDigit(const char c);
  static bool isZero(const char c);
  static bool isCharacter(const char c);
};
