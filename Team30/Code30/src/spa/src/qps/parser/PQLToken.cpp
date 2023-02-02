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
