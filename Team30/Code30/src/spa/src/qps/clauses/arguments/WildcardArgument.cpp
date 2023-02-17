#include "WildcardArgument.h"

StmtRef WildcardArgument::toStmtRef() {
  return StmtRef{StmtType::None, 0};
}

EntityRef WildcardArgument::toEntityRef() {
  return EntityRef{EntityType::None, ""};
}

bool WildcardArgument::isWildcard() {
  return true;
}
