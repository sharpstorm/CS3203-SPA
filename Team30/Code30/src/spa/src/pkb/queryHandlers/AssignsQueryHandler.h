#pragma once

#include <memory>

#include "common/Types.h"
#include "interfaces/IAssignsQueryHandler.h"

using std::shared_ptr;

class AssignsQueryHandler : public IAssignsQueryHandler {
 public:
  AssignsQueryHandler();

  QueryResult<int, shared_ptr<IASTNode>> queryAssigns() const override;
  QueryResult<int, shared_ptr<IASTNode>> queryAssigns(StmtRef) const override;
};
