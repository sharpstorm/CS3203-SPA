#pragma once

#include <string>
#include "pkb/PkbTypes.h"
#include "common/Types.h"
#include "pkb/storage/StructureMappingProvider.h"
#include "pkb/storage/EntityMappingProvider.h"

using std::string;

class PredicateFactory {
 private:
  const IStructureMappingProvider *structureProvider;
  const IEntityMappingProvider *entityProvider;

 public:
  explicit PredicateFactory(const IStructureMappingProvider *,
                            const IEntityMappingProvider *);

  Predicate<int> getPredicate(StmtRef) const;
  Predicate<string> getPredicate(EntityRef) const;
};
