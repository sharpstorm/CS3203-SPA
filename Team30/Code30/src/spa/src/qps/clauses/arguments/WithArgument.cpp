#include <utility>

#include "WithArgument.h"

WithArgument::WithArgument(int intVal) :
    intValue(intVal), identValue(NO_ENT_REF), syn(nullptr) { }

WithArgument::WithArgument(string identVal) :
    intValue(NO_STMT_REF), identValue(identVal), syn(nullptr) { }

WithArgument::WithArgument(AttributedSynonymPtr syn) :
    intValue(NO_STMT_REF), identValue(NO_ENT_REF), syn(std::move(syn)) { }

bool WithArgument::doesReturnInteger() {
  if (isSyn()) {
    return (syn->getAttribute() & INT_RETURN_MASK) > 0;
  }

  return intValue != NO_STMT_REF;
}

bool WithArgument::isSyn() {
  return syn != nullptr;
}

PQLSynonymName WithArgument::getSynName() {
  return syn->getName();
}

PQLSynonymType WithArgument::getSynType() {
  return syn->getType();
}

bool WithArgument::isAttributeValid() {
  if (!isSyn()) {
    return true;
  }

  return syn->validateAttribute();
}

PQLSynonymAttribute WithArgument::getAttribute() {
  if (!isSyn()) {
    return NO_ATTRIBUTE;
  }

  return syn->getAttribute();
}

bool WithArgument::isStaticValueEqual(const WithArgument &other) {
  // If either are syn values
  if (isSyn() || other.syn != nullptr) {
    return false;
  }

  return intValue == other.intValue && identValue == other.identValue;
}

const char WithArgument::NO_ENT_REF[] = "";

