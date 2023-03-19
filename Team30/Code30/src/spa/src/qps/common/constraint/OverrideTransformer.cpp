#include "OverrideTransformer.h"

StmtRef OverrideTransformer::transformArg(StmtRef input) {
  if (stmtRef == NO_STMT_REF) {
    return input;
  }
  return StmtRef { input.type, stmtRef };
}

EntityRef OverrideTransformer::transformArg(EntityRef input) {
  if (entRef == NO_ENT_REF) {
    return input;
  }
  return EntityRef { input.type, entRef };
}

bool OverrideTransformer::returnsInteger() {
  return stmtRef != NO_STMT_REF;
}

const char OverrideTransformer::NO_ENT_REF[] = "";

StmtValue OverrideTransformer::getStmtValue() {
  return stmtRef;
}

EntityValue OverrideTransformer::getEntityValue() {
  return entRef;
}
