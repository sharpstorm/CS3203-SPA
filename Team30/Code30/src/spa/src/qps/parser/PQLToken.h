#pragma once

#include <string>
#include <vector>

using std::string, std::vector;

typedef string TokenData;

enum PQLTokenCategory : uint16_t {
  PQL_PROCESSING_TOKEN = 0x100,
  PQL_SYMBOL_TOKEN = 0x200,
  PQL_DECLARATION_TOKEN = 0x400,
  PQL_QUERY_TOKEN = 0x800,
  PQL_RELATIONSHIP_TOKEN = 0x1000,
  PQL_STRING_TOKEN = 0x2000,
  PQL_INTEGER_TOKEN = 0x4000,
  PQL_LITERAL_TOKEN = 0x8000,
};

const uint16_t PQL_TOKEN_CATEGORY_MASK = 0xFF00;
const uint16_t PQL_TOKEN_SYN_MASK = PQL_DECLARATION_TOKEN | PQL_QUERY_TOKEN |
    PQL_RELATIONSHIP_TOKEN | PQL_STRING_TOKEN;
const uint16_t PQL_DATA_TOKEN_MASK = PQL_STRING_TOKEN | PQL_INTEGER_TOKEN |
    PQL_LITERAL_TOKEN;

enum PQLTokenType : uint16_t {
  // Processing markers
  PQL_TOKEN_NULL = PQL_PROCESSING_TOKEN,
  PQL_TOKEN_INVALID,
  PQL_TOKEN_IGNORE,
  PQL_TOKEN_DELIMITER,
  PQL_TOKEN_CHAR,
  PQL_TOKEN_QUOTE,
  PQL_TOKEN_VALID,

  // Symbols
  PQL_TOKEN_SEMICOLON = PQL_SYMBOL_TOKEN,
  PQL_TOKEN_BRACKET_OPEN,
  PQL_TOKEN_BRACKET_CLOSE,
  PQL_TOKEN_TUPLE_OPEN,
  PQL_TOKEN_TUPLE_CLOSE,
  PQL_TOKEN_COMMA,
  PQL_TOKEN_PERIOD,
  PQL_TOKEN_UNDERSCORE,
  PQL_TOKEN_ASTRIX,
  PQL_TOKEN_EQUALS,
  PQL_TOKEN_NUMBER_SIGN,

  // Declarations
  PQL_TOKEN_STMT = PQL_DECLARATION_TOKEN,
  PQL_TOKEN_READ,
  PQL_TOKEN_PRINT,
  PQL_TOKEN_CALL,
  PQL_TOKEN_WHILE,
  PQL_TOKEN_IF,
  PQL_TOKEN_ASSIGN,
  PQL_TOKEN_VARIABLE,
  PQL_TOKEN_CONSTANT,
  PQL_TOKEN_PROCEDURE,

  // Query Keywords
  PQL_TOKEN_SELECT = PQL_QUERY_TOKEN,
  PQL_TOKEN_SUCH,
  PQL_TOKEN_THAT,
  PQL_TOKEN_PATTERN,
  PQL_TOKEN_AND,
  PQL_TOKEN_WITH,
  PQL_TOKEN_PROC_NAME,
  PQL_TOKEN_VAR_NAME,
  PQL_TOKEN_VALUE,
  PQL_TOKEN_BOOLEAN,

  // Relationship Keywords
  PQL_TOKEN_FOLLOWS = PQL_RELATIONSHIP_TOKEN,
  PQL_TOKEN_PARENT,
  PQL_TOKEN_USES,
  PQL_TOKEN_MODIFIES,
  PQL_TOKEN_CALLS,
  PQL_TOKEN_NEXT,
  PQL_TOKEN_AFFECTS,

  PQL_TOKEN_STRING = PQL_STRING_TOKEN,
  PQL_TOKEN_INTEGER = PQL_INTEGER_TOKEN,
  PQL_TOKEN_LITERAL = PQL_LITERAL_TOKEN,
  PQL_TOKEN_STRING_LITERAL,
};

class PQLToken {
 private:
  PQLTokenType type;
  TokenData tokenData;

 public:
  explicit PQLToken(PQLTokenType type);
  PQLToken(PQLTokenType type, const TokenData &data);

  PQLTokenType getType() const;
  TokenData getData() const;
  bool isType(PQLTokenType) const;
  bool isCategory(PQLTokenCategory) const;
  bool isSynName() const;
  bool operator==(const PQLToken &other) const;
};

typedef vector<PQLToken> PQLTokenList;
