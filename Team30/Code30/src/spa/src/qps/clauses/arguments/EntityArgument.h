#pragma once

#include "ClauseArgument.h"

class EntityArgument : public ClauseArgument {
 private:
  EntityValue ident;

 public:
  explicit EntityArgument(const EntityValue &ident);
  StmtRef toStmtRef() const override;
  EntityRef toEntityRef() const override;
  bool isConstant() const override;
};
