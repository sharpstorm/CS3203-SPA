#pragma once

#include <memory>
#include "../../../common/ast/IASTNode.h"

class IAssignsQueryHandler {
 public:
  virtual ~IAssignsQueryHandler() {}
  virtual QueryResult<int, std::shared_ptr<IASTNode>> queryAssigns(
      StmtRef) const = 0;
};
