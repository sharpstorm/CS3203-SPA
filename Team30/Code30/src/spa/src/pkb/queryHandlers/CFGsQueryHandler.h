#pragma once

#include <memory>
#include <vector>

#include "../../common/Types.h"
#include "../../common/cfg/CFG.h"
#include "../storage/StorageTypes.h"
#include "../storage/StructureMappingProvider.h"
#include "interfaces/ICFGsQueryHandler.h"
#include "pkb/storage/interfaces/IEntityMappingProvider.h"

using std::shared_ptr, std::vector;

class CFGsQueryHandler : public ICFGsQueryHandler {
 public:
  explicit CFGsQueryHandler(const CFGStorage *cfgsStorage,
                            const IEntityMappingProvider *entityProvider,
                            const IStructureMappingProvider *structureProvider);
  vector<CFG*> queryCFGs(StmtRef) const override;

 private:
  const CFGStorage *cfgsStorage;
  const IStructureMappingProvider *structureProvider;
  const IEntityMappingProvider *entityProvider;
};
