#pragma once
#include "ClauseArgument.h"

class WildcardArgument: public ClauseArgument {
 public:
  WildcardArgument() = default;
  bool isWildcard() override;
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
};
