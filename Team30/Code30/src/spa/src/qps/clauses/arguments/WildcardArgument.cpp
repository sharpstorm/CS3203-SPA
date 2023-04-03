#include "WildcardArgument.h"

StmtRef WildcardArgument::toStmtRef() {
  return StmtRef{StmtType::Wildcard, 0};
}

EntityRef WildcardArgument::toEntityRef() {
  return EntityRef{EntityType::Wildcard, ""};
}

bool WildcardArgument::isWildcard() {
  return true;
}
