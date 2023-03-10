#include "WithArgument.h"

WithArgument::WithArgument(int intVal) :
    intValue(intVal), identValue(NO_ENT_REF), isSyn(false) { }

WithArgument::WithArgument(string identVal) :
    intValue(NO_STMT_REF), identValue(identVal), isSyn(false) { }

WithArgument::WithArgument(AttributedSynonym syn) :
    intValue(NO_STMT_REF), identValue(NO_ENT_REF), syn(syn), isSyn(true) { }

bool WithArgument::doesReturnInteger() {
  if (isSyn) {
    return (syn.getAttribute() & 0x20) > 0;
  }

  if (intValue == NO_STMT_REF) {
    return false;
  }

  return true;
}

bool WithArgument::getIsSyn() {
  return isSyn;
}

PQLSynonymName WithArgument::getSynName() {
  return syn.getName();
}

PQLSynonymType WithArgument::getSynType() {
  return syn.getType();
}

bool WithArgument::isAttributeValid() {
  if (!isSyn) {
    return false;
  }

  return syn.validateAttribute();
}

PQLSynonymAttribute WithArgument::getAttribute() {
  if (!isSyn) {
    return NO_ATTRIBUTE;
  }

  return syn.getAttribute();
}

const char WithArgument::NO_ENT_REF[] = "";
