#include <string>
#include "SourceToken.h"

SourceToken::SourceToken(SourceTokenType type, const TokenValue &value) :
    type(type), value(value) {
}

SourceTokenType SourceToken::getType() const {
  return this->type;
}

TokenValue SourceToken::getValue() const {
  return this->value;
}

bool SourceToken::operator==(const SourceToken &other) const {
  if (type == SIMPLE_TOKEN_VARIABLE || type == SIMPLE_TOKEN_INTEGER) {
    return type == other.type && value == other.value;
  }
  return type == other.type;
}

bool SourceToken::isCategory(SourceTokenType type,
                             SourceTokenCategory target) {
  return (type & SIMPLE_TOKEN_CATEGORY_MASK) == target;
}

bool SourceToken::isVarchar() const {
  return ((type & SIMPLE_TOKEN_CATEGORY_MASK) & SIMPLE_VARCHAR_MASK) > 0;
}
