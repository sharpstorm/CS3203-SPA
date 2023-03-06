#include "EntityArgument.h"
#include "qps/errors/QPSParserSyntaxError.h"

EntityArgument::EntityArgument(string ident): ident(ident) {
}

StmtRef EntityArgument::toStmtRef() {
  return StmtRef{StmtType::None, 0};
}

EntityRef EntityArgument::toEntityRef() {
  return EntityRef{EntityType::None, ident};
}
