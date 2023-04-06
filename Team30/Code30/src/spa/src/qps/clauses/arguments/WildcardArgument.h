#pragma once

#include "ClauseArgument.h"

class WildcardArgument : public ClauseArgument {
 public:
  WildcardArgument() = default;
  bool isWildcard() const override;
  StmtRef toStmtRef() const override;
  EntityRef toEntityRef() const override;
};
