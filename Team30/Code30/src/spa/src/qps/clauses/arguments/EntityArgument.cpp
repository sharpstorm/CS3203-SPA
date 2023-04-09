#include "EntityArgument.h"

EntityArgument::EntityArgument(const EntityValue &ident) : ident(ident) {
}

StmtRef EntityArgument::toStmtRef() const {
  return StmtRef{StmtType::None, NO_STMT};
}

EntityRef EntityArgument::toEntityRef() const {
  return EntityRef{EntityType::None, ident};
}

bool EntityArgument::isConstant() const {
  return true;
}
