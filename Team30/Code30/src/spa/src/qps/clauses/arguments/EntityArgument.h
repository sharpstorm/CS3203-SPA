#pragma once

#include <string>

#include "ClauseArgument.h"

using std::string;

class EntityArgument: public ClauseArgument {
 private:
  string ident;

 public:
  explicit EntityArgument(const string &ident);
  StmtRef toStmtRef() override;
  EntityRef toEntityRef() override;
  bool isConstant() override;
};
