#include <utility>

#include "WithArgument.h"
#include "common/Types.h"

WithArgument::WithArgument(const int &intVal) :
    intValue(intVal), identValue(NO_ENT), syn(nullptr) { }

WithArgument::WithArgument(const string &identVal) :
    intValue(NO_INT_VAL), identValue(identVal), syn(nullptr) { }

WithArgument::WithArgument(AttributedSynonymPtr syn) :
    intValue(NO_INT_VAL), identValue(NO_ENT), syn(std::move(syn)) { }

bool WithArgument::doesReturnInteger() {
  if (isSyn()) {
    return syn->returnsInteger();
  }

  return intValue != NO_INT_VAL;
}

bool WithArgument::isSyn() const {
  return syn != nullptr;
}

PQLSynonymName WithArgument::getSynName() {
  return syn->getName();
}

bool WithArgument::isSynStatement() {
  return syn->isStatementType();
}

bool WithArgument::isSameSynTypeAs(const WithArgument *other) {
  if (!isSyn() || !other->isSyn()) {
    return false;
  }

  return syn->getType() == other->syn->getType();
}

bool WithArgument::isStaticValueEqual(const WithArgument &other) {
  // If either are syn values
  if (isSyn() || other.isSyn()) {
    return false;
  }

  return intValue == other.intValue && identValue == other.identValue;
}

int WithArgument::toConstInt() {
  return intValue;
}

string WithArgument::toConstIdent() {
  return identValue;
}

AttributedSynonym WithArgument::toAttrSyn() {
  return *syn;
}

bool WithArgument::isDefaultAttribute() {
  return syn->isDefaultAttribute();
}
