#include "WildcardArgument.h"

StmtRef WildcardArgument::toStmtRef() const {
  return StmtRef{StmtType::Wildcard, NO_STMT};
}

EntityRef WildcardArgument::toEntityRef() const {
  return EntityRef{EntityType::Wildcard, NO_ENT};
}

bool WildcardArgument::isWildcard() const {
  return true;
}
