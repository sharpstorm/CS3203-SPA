#include "OverrideTransformer.h"

#include <string>

using std::to_string;

OverrideTransformer::OverrideTransformer(const IntegerValue &intVal) :
    intValue(intVal), entValue(NO_ENT) {}

OverrideTransformer::OverrideTransformer(const EntityValue &ent) :
    intValue(NO_INT_VAL), entValue(ent) {}

StmtRef OverrideTransformer::transformArg(const StmtRef &input) const {
  if (intValue == NO_INT_VAL) {
    return input;
  }

  if (intValue == NO_STMT) {
    return StmtRef { input.getType(), INVALID_STMT };
  }

  return StmtRef { input.getType(), intValue };
}

EntityRef OverrideTransformer::transformArg(const EntityRef &input) const {
  if (input.getType() == EntityType::Constant) {
    if (intValue == NO_INT_VAL) {
      return input;
    }
    return EntityRef{EntityType::Constant, to_string(intValue)};
  }

  if (entValue == NO_ENT) {
    return input;
  }
  return EntityRef{input.getType(), entValue};
}

bool OverrideTransformer::returnsInteger() const {
  return intValue != NO_INT_VAL;
}

bool OverrideTransformer::operator==(const OverrideTransformer &other) const {
  return intValue == other.intValue && entValue == other.entValue;
}
