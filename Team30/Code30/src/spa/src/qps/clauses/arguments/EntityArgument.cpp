#include "EntityArgument.h"
#include "qps/errors/QPSParserSyntaxError.h"

EntityArgument::EntityArgument(string ident): ident(ident) {
}

StmtRef EntityArgument::toStmtRef() {
  throw QPSParserSyntaxError("Invalid call to toStmtRef");
}

EntityRef EntityArgument::toEntityRef() {
  return EntityRef{EntityType::None, ident};
}
