#include <utility>

#include "WithArgument.h"
#include "common/Types.h"

WithArgument::WithArgument(const StmtValue &intVal) :
    intValue(intVal), identValue(NO_ENT), syn(nullptr) {}

WithArgument::WithArgument(const EntityValue &identVal) :
    intValue(NO_STMT), identValue(identVal), syn(nullptr) {}

WithArgument::WithArgument(AttributedSynonymPtr syn) :
    intValue(NO_STMT), identValue(NO_ENT), syn(std::move(syn)) {}

bool WithArgument::doesReturnInteger() const {
  if (isSyn()) {
    return syn->returnsInteger();
  }

  return intValue != NO_STMT;
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

StmtValue WithArgument::toConstInt() const {
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
