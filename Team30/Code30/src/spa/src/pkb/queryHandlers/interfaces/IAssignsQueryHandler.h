#pragma once

#include <memory>
#include "../../../common/ASTNode/IASTNode.h"

class IAssignsQueryHandler {
 public:
  virtual ~IAssignsQueryHandler() {}
  virtual QueryResult<int, std::shared_ptr<IASTNode>> queryAssigns() const = 0;
  virtual QueryResult<int, std::shared_ptr<IASTNode>> queryAssigns(
      StmtRef) const = 0;
};
