#include <utility>

#include "WithArgument.h"
#include "common/Types.h"

WithArgument::WithArgument(const IntegerValue &intVal) :
    intValue(intVal), identValue(NO_ENT), syn(nullptr) {}

WithArgument::WithArgument(const EntityValue &identVal) :
    intValue(NO_INT_VAL), identValue(identVal), syn(nullptr) { }

WithArgument::WithArgument(AttributedSynonymPtr syn) :
    intValue(NO_INT_VAL), identValue(NO_ENT), syn(std::move(syn)) { }

bool WithArgument::returnsInteger() const {
  if (isSyn()) {
    return syn->returnsInteger();
  }

  return intValue != NO_INT_VAL;
}

bool WithArgument::isSyn() const {
  return syn != nullptr;
}

PQLSynonymName WithArgument::getSynName() const {
  return syn->getName();
}

bool WithArgument::isSynStatement() const {
  return syn->isStatementType();
}

bool WithArgument::isSameSynTypeAs(const WithArgument *other) const {
  if (!isSyn() || !other->isSyn()) {
    return false;
  }

  return syn->getType() == other->syn->getType();
}

bool WithArgument::isStaticValueEqual(const WithArgument &other) const {
  // If either are syn values
  if (isSyn() || other.isSyn()) {
    return false;
  }

  return intValue == other.intValue && identValue == other.identValue;
}

IntegerValue WithArgument::toConstInt() const {
  return intValue;
}

EntityValue WithArgument::toConstIdent() const {
  return identValue;
}

AttributedSynonym WithArgument::toAttrSyn() const {
  return *syn;
}

bool WithArgument::isDefaultAttribute() const {
  return syn->isDefaultAttribute();
}
