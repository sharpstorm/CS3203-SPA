#include "Types.h"

StmtRef::StmtRef(StmtType type, StmtValue lineNum) : IRef(type, lineNum) {}

bool StmtRef::isKnown() const {
  return getValue() != NO_STMT;
}

bool StmtRef::isValid() const {
  return getValue() >= NO_STMT;
}

EntityRef::EntityRef(EntityType type) : IRef(type, NO_ENT) {}

EntityRef::EntityRef(EntityType type, const EntityValue &name) :
    IRef(type, name) {}

bool EntityRef::isKnown() const {
  return getValue() != NO_ENT;
}
