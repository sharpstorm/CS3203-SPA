#include "OverrideTransformer.h"

StmtRef OverrideTransformer::transformArg(StmtRef input) {
  if (stmtRef == NO_STMT_REF) {
    return input;
  }
  return StmtRef { StmtType::None, stmtRef };
}

EntityRef OverrideTransformer::transformArg(EntityRef input) {
  if (entRef == NO_ENT_REF) {
    return input;
  }
  return EntityRef { EntityType::None, entRef };
}
