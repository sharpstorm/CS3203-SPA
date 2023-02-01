#pragma once

#include <unordered_map>
#include <string>
#include "../PQLToken.h"

using std::string, std::unordered_map;

const char PQL_KEYWORD_SELECT[] = "Select";

const char PQL_KEYWORD_STMT[] = "stmt";
const char PQL_KEYWORD_READ[] = "read";
const char PQL_KEYWORD_PRINT[] = "print";
const char PQL_KEYWORD_CALL[] = "call";
const char PQL_KEYWORD_WHILE[] = "while";
const char PQL_KEYWORD_IF[] = "if";
const char PQL_KEYWORD_ASSIGN[] = "assign";
const char PQL_KEYWORD_VARIABLE[] = "variable";
const char PQL_KEYWORD_CONSTANT[] = "constant";
const char PQL_KEYWORD_PROCEDURE[] = "procedure";

const char PQL_KEYWORD_SUCH[] = "such";
const char PQL_KEYWORD_THAT[] = "that";

const char PQL_KEYWORD_FOLLOWS[] = "Follows";
const char PQL_KEYWORD_PARENT[] = "Parent";
const char PQL_KEYWORD_USES[] = "Uses";
const char PQL_KEYWORD_MODIFIES[] = "Modifies";
const char PQL_KEYWORD_PATTERN[] = "pattern";

class QueryLexerTokenTable {
 public:
  unordered_map<string, PQL_TOKEN_TYPE> keywordMap = {
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
  };
  PQL_TOKEN_TYPE tokens[256];
  QueryLexerTokenTable();
};
