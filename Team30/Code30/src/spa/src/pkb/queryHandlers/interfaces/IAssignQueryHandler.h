#pragma once

#include "../../../common/Types.h"
#include "../../../common/ASTNode/ASTNode.h"

class IAssignQueryHandler {
 public:
  virtual ~IAssignQueryHandler() {}
  virtual QueryResult<int, ASTNode> queryAssigns() const = 0;
  virtual QueryResult<int, ASTNode> queryAssignsStar(StmtRef) const = 0;
};
