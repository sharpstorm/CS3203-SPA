#include <utility>

#include "WithArgument.h"
#include "common/Types.h"

WithArgument::WithArgument(const int &intVal) :
    intValue(intVal), identValue(NO_ENT), syn(nullptr) { }

WithArgument::WithArgument(const string &identVal) :
    intValue(NO_STMT), identValue(identVal), syn(nullptr) { }

WithArgument::WithArgument(AttributedSynonymPtr syn) :
    intValue(NO_STMT), identValue(NO_ENT), syn(std::move(syn)) { }

bool WithArgument::doesReturnInteger() {
  if (isSyn()) {
    return syn->returnsInteger();
  }

  return intValue != NO_STMT;
}

bool WithArgument::isSyn() const {
  return syn != nullptr;
}

PQLSynonymName WithArgument::getSynName() {
  return syn->getName();
}

PQLSynonymType WithArgument::getSynType() {
  return syn->getType();
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

bool WithArgument::isDefaultAttribute() {
  return syn->isDefaultAttribute();
}
