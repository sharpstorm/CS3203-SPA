#pragma once

#include <string>
#include <unordered_set>
#include "common/Types.h"
#include "interfaces/IModifiesQueryHandler.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

using std::string;
using std::unordered_set;

class ModifiesQueryHandler : public IModifiesQueryHandler {
 public:
  ModifiesQueryHandler(const ModifiesStorage *,
                       const ModifiesPStorage *,
                       const PredicateFactory *,
                       const IStructureMappingProvider *,
                       const IEntityMappingProvider *);

  QueryResult<int, string> queryModifies(StmtRef, EntityRef) const override;
  QueryResult<string, string> queryModifies(EntityRef,
                                            EntityRef) const override;
 private:
  const ModifiesStorage *modifiesStorage;
  const ModifiesPStorage *modifiesPStorage;
  const PredicateFactory *predicateFactory;
  const IStructureMappingProvider *structureProvider;
  const IEntityMappingProvider *entitiesProvider;

  bool validateArg1(StmtRef) const;
  bool validateArg1(EntityRef) const;
  bool validateArg2(EntityRef) const;
};
