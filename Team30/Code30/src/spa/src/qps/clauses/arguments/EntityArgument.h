#pragma once

#include <string>
#include "ClauseArgument.h"

using std::string;

class EntityArgument: public ClauseArgument {
 private:
  string ident;

 public:
  EntityArgument(string ident);
  StmtRef toStmtRef();
  EntityRef toEntityRef();
};
