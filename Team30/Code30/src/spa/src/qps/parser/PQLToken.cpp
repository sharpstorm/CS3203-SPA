#include "PQLToken.h"

PQLToken::PQLToken(PQLTokenType type):
    type(type) {
}

PQLToken::PQLToken(PQLTokenType type, string data):
    type(type), tokenData(data) {
}

bool PQLToken::operator==(const PQLToken& other) const {
  if (type == PQL_TOKEN_STRING || type == PQL_TOKEN_INTEGER
      || type == PQL_TOKEN_LITERAL || type == PQL_TOKEN_STRING_LITERAL) {
    return type == other.type && tokenData == other.tokenData;
  }
  return type == other.type;
}

PQLTokenType PQLToken::getType() {
  return type;
}

string PQLToken::getData() {
  return tokenData;
}

bool PQLToken::isType(PQLTokenType target) {
  return type == target;
}

bool PQLToken::isVarchar() {
  return ((type & PQL_TOKEN_CATEGORY_MASK)
    & PQL_TOKEN_VARCHAR_MASK) > 0;
}

bool PQLToken::isSynName() {
  return ((type & PQL_TOKEN_CATEGORY_MASK)
      & PQL_TOKEN_SYN_MASK) > 0;
}

bool PQLToken::isCategory(PQLTokenCategory category) {
  return (type & PQL_TOKEN_CATEGORY_MASK) == category;
}
