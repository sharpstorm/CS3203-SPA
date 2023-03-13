#pragma once

#include <memory>

#include "../../common/Types.h"
#include "../../common/cfg/CFG.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"
#include "interfaces/ICFGsQueryHandler.h"

using std::shared_ptr;

class CFGsQueryHandler : public ICFGsQueryHandler {
 public:
  explicit CFGsQueryHandler(const CFGStorage *cfgsStorage,
                            const IStructureMappingProvider *structureProvider);
  QueryResult<int, CFG *> queryCFGs(StmtRef) const override;

 private:
  const CFGStorage *cfgsStorage;
  const IStructureMappingProvider *structureProvider;
};
