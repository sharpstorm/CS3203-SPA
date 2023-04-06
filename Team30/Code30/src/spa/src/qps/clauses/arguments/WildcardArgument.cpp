#include "WildcardArgument.h"

StmtRef WildcardArgument::toStmtRef() const {
  return StmtRef{StmtType::Wildcard, 0};
}

EntityRef WildcardArgument::toEntityRef() const {
  return EntityRef{EntityType::Wildcard, ""};
}

bool WildcardArgument::isWildcard() const {
  return true;
}
