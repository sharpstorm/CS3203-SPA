#pragma once
#include "ClauseArgument.h"

class WildcardArgument: public ClauseArgument {
 public:
  WildcardArgument() {}
  bool isWildcard() override;
  StmtRef toStmtRef();
  EntityRef toEntityRef();
};
