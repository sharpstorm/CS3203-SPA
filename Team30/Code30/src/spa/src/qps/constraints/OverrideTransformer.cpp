#include "OverrideTransformer.h"

#include <string>

using std::to_string;

StmtRef OverrideTransformer::transformArg(StmtRef input) const {
  if (stmtRef == NO_STMT) {
    return input;
  }
  return StmtRef { input.getType(), stmtRef };
}

EntityRef OverrideTransformer::transformArg(EntityRef input) const {
  if (input.getType() == EntityType::Constant) {
    if (stmtRef == NO_STMT) {
      return input;
    }
    return EntityRef {EntityType::Constant, to_string(stmtRef) };
  }

  if (entRef == NO_ENT) {
    return input;
  }
  return EntityRef { input.getType(), entRef };
}

bool OverrideTransformer::returnsInteger() const {
  return stmtRef != NO_STMT;
}