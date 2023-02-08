#include <string>
#include "SourceToken.h"

SourceToken::SourceToken(SourceTokenType type, string value):
    type(type), value(value) {
}

SourceTokenType SourceToken::getType() {
  return this->type;
}

string SourceToken::getValue() {
  return this->value;
}

bool SourceToken::operator==(const SourceToken &other) const {
  if (type == SIMPLE_TOKEN_VARIABLE || type == SIMPLE_TOKEN_INTEGER) {
    return type == other.type && value == other.value;
  }
  return type == other.type;
}

bool SourceToken::isType(SourceTokenType target) {
  return type == target;
}

bool SourceToken::isCategory(SourceTokenCategory target) {
  return isCategory(type, target);
}

bool SourceToken::isCategory(SourceTokenType type,
                                    SourceTokenCategory target) {
  return (type & SIMPLE_TOKEN_CATEGORY_MASK) == target;
}

bool SourceToken::isVarchar() {
  return ((type & SIMPLE_TOKEN_CATEGORY_MASK) & SIMPLE_VARCHAR_MASK) > 0;
}
