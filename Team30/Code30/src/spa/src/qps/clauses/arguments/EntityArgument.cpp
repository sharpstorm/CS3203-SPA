#include "EntityArgument.h"

EntityArgument::EntityArgument(const string &ident): ident(ident) {
}

StmtRef EntityArgument::toStmtRef() {
  return StmtRef{StmtType::None, 0};
}

EntityRef EntityArgument::toEntityRef() {
  return EntityRef{EntityType::None, ident};
}

bool EntityArgument::isConstant() {
  return true;
}
