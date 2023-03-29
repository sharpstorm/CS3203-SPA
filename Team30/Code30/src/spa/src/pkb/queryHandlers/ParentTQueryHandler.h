#pragma once

#include "BaseQueryHandler.h"
#include "PkbStmtStmtQueryInvoker.h"
#include "common/Types.h"
#include "pkb/queryHandlers/interfaces/IParentTQueryHandler.h"
#include "pkb/storage/StorageTypes.h"

class ParentTQueryHandler : public IParentTQueryHandler {
 public:
  ParentTQueryHandler(ParentTStorage *storage,
                      IStructureMappingProvider *provider,
                      StmtPredicateFactory *factory);
  QueryResultPtr<StmtValue, StmtValue> query(StmtRef *leftArg,
                                             StmtRef *rightArg) const override;

 private:
  ParentTStorage *store;
  IStructureMappingProvider *provider;
  StmtPredicateFactory *factory;
};
