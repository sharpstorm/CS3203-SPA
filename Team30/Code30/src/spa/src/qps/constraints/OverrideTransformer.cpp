#include "OverrideTransformer.h"

#include <string>

using std::to_string;

OverrideTransformer::OverrideTransformer(const StmtValue &stmt) :
    stmtRef(stmt), entRef(NO_ENT) {}

OverrideTransformer::OverrideTransformer(const EntityValue &ent) :
    stmtRef(NO_INT_VAL), entRef(ent) {}

StmtRef OverrideTransformer::transformArg(const StmtRef &input) const {
  if (stmtRef == NO_INT_VAL) {
    return input;
  }

  if (stmtRef == NO_STMT) {
    return StmtRef { input.getType(), NO_INT_VAL };
  }

  return StmtRef { input.getType(), stmtRef };
}

EntityRef OverrideTransformer::transformArg(const EntityRef &input) const {
  if (input.getType() == EntityType::Constant) {
    if (stmtRef == NO_INT_VAL) {
      return input;
    }
    return EntityRef{EntityType::Constant, to_string(stmtRef)};
  }

  if (entRef == NO_ENT) {
    return input;
  }
  return EntityRef{input.getType(), entRef};
}

bool OverrideTransformer::returnsInteger() const {
  return stmtRef != NO_INT_VAL;
}

bool OverrideTransformer::operator==(const OverrideTransformer &other) const {
  return stmtRef == other.stmtRef && entRef == other.entRef;
}
