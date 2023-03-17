#pragma once

#include <string>
#include <unordered_set>

#include "common/Types.h"
#include "interfaces/IUsesQueryHandler.h"
#include "pkb/predicates/PredicateFactory.h"
#include "pkb/storage/StorageTypes.h"
#include "pkb/storage/interfaces/IStructureMappingProvider.h"

class UsesQueryHandler : public IUsesQueryHandler {
 public:
  UsesQueryHandler(const UsesStorage *, const UsesPStorage *,
                   const PredicateFactory *, const IStructureMappingProvider *,
                   const IEntityMappingProvider *);

  QueryResult<int, string> queryUses(StmtRef, EntityRef) const override;
  QueryResult<string, string> queryUses(EntityRef, EntityRef) const override;
  string getPrintDeclarations(int) const override;

 private:
  const UsesStorage *usesStorage;
  const UsesPStorage *usesPStorage;
  const PredicateFactory *predicateFactory;
  const IStructureMappingProvider *structureProvider;
  const IEntityMappingProvider *entitiesProvider;

  bool validateArg1(StmtRef) const;
  bool validateArg1(EntityRef) const;
  bool validateArg2(EntityRef) const;
};
