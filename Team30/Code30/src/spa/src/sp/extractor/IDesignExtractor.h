#pragma once

#include "common/AST.h"
class ISp {
 public:
  virtual void extractRelations(AST ast) = 0;
  virtual void extractEntities(AST ast) = 0;
};
