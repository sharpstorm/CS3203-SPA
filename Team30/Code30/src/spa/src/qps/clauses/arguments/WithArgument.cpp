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
    return syn->returnsInteger();
  }

  return intValue != NO_STMT_REF;
}

bool WithArgument::isSyn() const {
  return syn != nullptr;
}

PQLQuerySynonymProxy WithArgument::getSyn() {
  return syn->getSynProxy();
}

PQLSynonymName WithArgument::getSynName() {
  return syn->getName();
}

PQLSynonymType WithArgument::getSynType() {
  return syn->getType();
}

bool WithArgument::isSynType(PQLSynonymType type) {
  return syn->getType() == type;
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
  if (isSyn() || other.isSyn()) {
    return false;
  }

  return intValue == other.intValue && identValue == other.identValue;
}

int WithArgument::getIntValue() {
  return intValue;
}

string WithArgument::getIdentValue() {
  return identValue;
}

AttributedSynonym WithArgument::getAttrSyn() {
  return *syn;
}

const char WithArgument::NO_ENT_REF[] = "";



