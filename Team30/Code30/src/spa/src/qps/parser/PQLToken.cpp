#include "PQLToken.h"

bool PQLToken::operator==(const PQLToken& other) const {
  if (type == PQL_TOKEN_STRING || type == PQL_TOKEN_INTEGER) {
    return type == other.type && tokenData == other.tokenData;
  }
  return type == other.type;
}

bool PQLToken::isType(PQLTokenType target) {
  return type == target;
}

bool PQLToken::isCategory(PQLTokenCategory target) {
  return (type & PQL_TOKEN_CATEGORY_MASK) == target;
}

bool PQLToken::isVarchar() {
  return ((type & PQL_TOKEN_CATEGORY_MASK)
    & PQL_TOKEN_VARCHAR_MASK) > 0;
}
