#include "OverrideTransformer.h"

#include <string>

using std::to_string;

StmtRef OverrideTransformer::transformArg(StmtRef input) {
  if (stmtRef == NO_STMT_REF) {
    return input;
  }
  return StmtRef { input.getType(), stmtRef };
}

EntityRef OverrideTransformer::transformArg(EntityRef input) {
  if (input.getType() == EntityType::Constant) {
    if (stmtRef == NO_STMT_REF) {
      return input;
    }
    return EntityRef {EntityType::Constant, to_string(stmtRef) };
  }

  if (entRef == NO_ENT_REF) {
    return input;
  }
  return EntityRef { input.getType(), entRef };
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
