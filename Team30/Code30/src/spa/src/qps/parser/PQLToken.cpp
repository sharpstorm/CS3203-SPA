#include "PQLToken.h"

PQLToken::PQLToken(PQLTokenType type) :
    type(type) {
}

PQLToken::PQLToken(PQLTokenType type, const TokenData &data) :
    type(type), tokenData(data) {
}

bool PQLToken::operator==(const PQLToken &other) const {
  if ((type & PQL_DATA_TOKEN_MASK) > 0) {
    return type == other.type && tokenData == other.tokenData;
  }
  return type == other.type;
}

PQLTokenType PQLToken::getType() const {
  return type;
}

TokenData PQLToken::getData() const {
  return tokenData;
}

bool PQLToken::isType(PQLTokenType target) const {
  return type == target;
}

bool PQLToken::isSynName() const {
  return ((type & PQL_TOKEN_CATEGORY_MASK)
      & PQL_TOKEN_SYN_MASK) > 0;
}

bool PQLToken::isCategory(PQLTokenCategory category) const {
  return (type & PQL_TOKEN_CATEGORY_MASK) == category;
}
