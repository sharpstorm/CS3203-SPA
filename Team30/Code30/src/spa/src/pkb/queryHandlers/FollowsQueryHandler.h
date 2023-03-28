#pragma once

#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"
#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IFollowsQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

class FollowsQueryHandler : public IFollowsQueryHandler {
 public:
  FollowsQueryHandler(FollowsStorage*, IStructureMappingProvider*,
                      StmtPredicateFactory*);

  QueryResultPtr<StmtValue, StmtValue> query(StmtRef*, StmtRef*) const override;

 private:
  FollowsStorage* store;
  IStructureMappingProvider* provider;
  StmtPredicateFactory* factory;
};
